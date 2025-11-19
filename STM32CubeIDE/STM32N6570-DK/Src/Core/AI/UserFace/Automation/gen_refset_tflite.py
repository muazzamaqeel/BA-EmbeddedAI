#!/usr/bin/env python3
import argparse, os, glob, pathlib, subprocess, struct
import numpy as np
from PIL import Image
import warnings
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

# ==============================
# AES configuration (must match STM32)
# ==============================
AES_KEY = bytes.fromhex("603DEB1015CA71BE2B73AEF0857D7781")  # 128-bit key
AES_IV  = bytes.fromhex("000102030405060708090A0B0C0D0E0F")   # 16-byte IV


# ==============================
# AES encryption helper
# ==============================
def encrypt_bytes(data: bytes) -> bytes:
    """
    Encrypt raw bytes using AES-CBC with PKCS#7 padding ALWAYS applied.
    (STM32 decryptor expects padded blocks even if already aligned)
    """
    print(f"[AES] Raw length: {len(data)} bytes")

    # Create AES cipher context
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)

    # Always pad, even for block-aligned data
    padded = pad(data, 16)
    print(f"[AES] Padded length: {len(padded)} bytes (added {len(padded) - len(data)} bytes padding)")

    # Encrypt
    enc = cipher.encrypt(padded)

    print(f"[AES] Encrypted length: {len(enc)} bytes")
    print(f"[AES] First 16 bytes ciphertext: {enc[:16].hex().upper()}")
    return enc



# ==============================
# TensorFlow Lite import
# ==============================
try:
    import tflite_runtime.interpreter as tflite
    _TFLITE = True
    print("[TFLITE] Using tflite-runtime interpreter.")
except Exception:
    import tensorflow as tf
    warnings.filterwarnings("ignore", message=".*tf.lite.Interpreter is deprecated.*")
    tflite = tf.lite
    _TFLITE = False
    print("[TFLITE] Using full TensorFlow Lite interpreter.")


# ==============================
# Utility functions
# ==============================
def get_qparams(tensor_detail):
    if "quantization" in tensor_detail and tensor_detail["quantization"]:
        sc, zp = tensor_detail["quantization"]
        return float(sc), int(zp)
    qp = tensor_detail.get("quantization_parameters", None)
    if qp and len(qp.get("scales", [])) == 1 and len(qp.get("zero_points", [])) == 1:
        return float(qp["scales"][0]), int(qp["zero_points"][0])
    return 0.0, 0


def center_square_resize_to_minus1_1(path, size=(160, 160)):
    print(f"[IMG] Loading {path}")
    im = Image.open(path).convert("RGB")
    w, h = im.size
    side = min(w, h)
    x0, y0 = (w - side) // 2, (h - side) // 2
    im = im.crop((x0, y0, x0 + side, y0 + side)).resize(size, Image.BILINEAR)
    arr = np.asarray(im, dtype=np.float32)
    arr = (arr - 127.0) / 128.0
    print(f"[IMG] Shape after resize: {arr.shape}, range=({arr.min():.2f},{arr.max():.2f})")
    return arr


def prepare_input_tensor(x_minus1_1_hwc, in_shape, in_dtype, in_scale, in_zp):
    if len(in_shape) != 4:
        raise SystemExit(f"Unexpected input rank: {in_shape}")
    n, d1, d2, d3 = in_shape
    layout = "NHWC" if d3 == 3 else "NCHW" if d1 == 3 else None
    if not layout:
        raise SystemExit(f"Cannot infer layout from shape {in_shape}")

    print(f"[TENSOR] Preparing input tensor layout={layout}, dtype={in_dtype}")
    if in_dtype == np.uint8:
        q = np.round(x_minus1_1_hwc / in_scale + in_zp).clip(0, 255).astype(np.uint8)
    elif in_dtype == np.int8:
        q = np.round(x_minus1_1_hwc / in_scale + in_zp).clip(-128, 127).astype(np.int8)
    elif in_dtype in (np.float32, np.float16):
        q = x_minus1_1_hwc.astype(np.float32)
    else:
        raise SystemExit(f"Unsupported dtype: {in_dtype}")

    q = q[None, ...] if layout == "NHWC" else np.transpose(q, (2, 0, 1))[None, ...]
    print(f"[TENSOR] Final input tensor shape={q.shape}")
    return q


def dequantize_output(y_raw, out_dtype, out_scale, out_zp):
    if out_dtype == np.int8:
        return (y_raw.astype(np.float32) - float(out_zp)) * float(out_scale)
    if out_dtype in (np.float32, np.float16):
        return y_raw.astype(np.float32)
    raise SystemExit(f"Unsupported output dtype: {out_dtype}")


def l2_normalize(v, eps=1e-9):
    n = np.sqrt(np.sum(v * v))
    print(f"[NORM] L2 norm before normalize: {n:.6f}")
    return v / (n + eps)


# ==============================
# BIN file emitter (embeddings)
# ==============================
def emit_bin(out_bin, subject, vecs, encrypt_fn):
    print(f"[CGEN] Writing BIN → {out_bin}")
    name = subject.encode("utf-8")
    name_len = len(name)
    emb_dim = len(vecs[0])
    n_emb = len(vecs)

    with open(out_bin, "wb") as f:
        f.write(b"FREB")
        f.write(struct.pack("<H", 1))           # version
        f.write(struct.pack("<H", name_len))
        f.write(struct.pack("<H", emb_dim))
        f.write(struct.pack("<H", n_emb))
        f.write(name)
        for i, v in enumerate(vecs):
            raw = np.asarray(v, dtype=np.float32).tobytes()
            print(f"[CGEN] Encrypting embedding {i}: raw len={len(raw)}")
            enc = encrypt_fn(raw)
            f.write(struct.pack("<I", len(enc)))
            f.write(enc)
            print(f"[CGEN] -> wrote enc_len={len(enc)}")
    print(f"[CGEN] ✅ BIN written: {out_bin}  (emb_dim={emb_dim}, n_emb={n_emb})")


# ==============================
# PIN encryption emitter
# ==============================
def emit_pin_bin(pin_path, out_bin, subject, encrypt_fn):
    if not os.path.exists(pin_path):
        print(f"[PIN] No pin.txt found for {subject}, skipping.")
        return
    with open(pin_path, "r", encoding="utf-8") as f:
        pin_value = f.read().strip()
    if not pin_value:
        print(f"[PIN] Empty pin.txt for {subject}, skipping.")
        return

    print(f"[PIN] Encrypting PIN for {subject}: '{pin_value}'")
    raw = pin_value.encode("utf-8")
    enc = encrypt_fn(raw)

    with open(out_bin, "wb") as f:
        f.write(b"FPIN")                     # header tag
        f.write(struct.pack("<H", 1))        # version
        f.write(struct.pack("<H", len(subject)))
        f.write(subject.encode("utf-8"))
        f.write(struct.pack("<I", len(enc)))
        f.write(enc)
    print(f"[PIN] ✅ PIN BIN written: {out_bin} (len={len(enc)} bytes)")


# ==============================
# Main Function
# ==============================
def main():
    print("\n=== Face Embedding Encryption Tool → BIN per person ===")
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default="facenet_512_int_quant.tflite")
    ap.add_argument("--in_dir", default=".")
    ap.add_argument("--subject", default="Muazzam")
    ap.add_argument("--no_l2norm", action="store_true")
    ap.add_argument("--save_npz", action="store_true")
    ap.add_argument("--skip_auto", action="store_true")
    ap.add_argument("--out_root", default="all_faces", help="Output root directory")
    args = ap.parse_args()

    root = os.path.dirname(os.path.abspath(__file__))
    all_faces_root = os.path.join(root, args.out_root)
    os.makedirs(all_faces_root, exist_ok=True)
    person_dir = os.path.join(all_faces_root, args.subject)
    os.makedirs(person_dir, exist_ok=True)

    out_txt = os.path.join(person_dir, f"embeddings_{args.subject}.txt")
    out_bin = os.path.join(person_dir, f"{args.subject}.bin")
    out_npz = os.path.join(person_dir, f"embeddings_{args.subject}.npz")
    out_pin_bin = os.path.join(person_dir, f"{args.subject.lower()}_pin.bin")
    pin_txt = os.path.join(args.in_dir, "pin.txt")

    print(f"[ARGS] Subject={args.subject}")
    print("[MODEL] Loading TFLite model...")
    interp = tflite.Interpreter(model_path=args.model)
    interp.allocate_tensors()
    in_det, out_det = interp.get_input_details()[0], interp.get_output_details()[0]
    in_idx, out_idx = in_det["index"], out_det["index"]
    in_dtype, out_dtype = in_det["dtype"], out_det["dtype"]
    in_shape = list(in_det["shape"])
    in_scale, in_zp = get_qparams(in_det)
    out_scale, out_zp = get_qparams(out_det)

    print(f"[IO] IN : dtype={in_dtype}, shape={in_shape}, scale={in_scale}, zp={in_zp}")
    print(f"[IO] OUT: dtype={out_dtype}, scale={out_scale}, zp={out_zp}")

    exts = ("*.bmp", "*.png", "*.jpg", "*.jpeg")
    files = sorted(sum([glob.glob(os.path.join(args.in_dir, e)) for e in exts], []))
    if not files:
        raise SystemExit(f"No images found in {args.in_dir}")
    print(f"[INFO] Found {len(files)} image(s) for subject '{args.subject}'")

    vecs, labels = [], []
    with open(out_txt, "w", encoding="utf-8") as outf:
        for fp in files:
            print(f"\n[PROC] Processing: {fp}")
            x = center_square_resize_to_minus1_1(fp, size=(160, 160))
            tin = prepare_input_tensor(x, in_shape, in_dtype, in_scale, in_zp)
            interp.set_tensor(in_idx, tin)
            interp.invoke()
            y_raw = interp.get_tensor(out_idx).reshape(-1)
            y = dequantize_output(y_raw, out_dtype, out_scale, out_zp)
            if not args.no_l2norm:
                y = l2_normalize(y)
            vecs.append(y.tolist())
            label = pathlib.Path(fp).stem
            labels.append(label)
            outf.write(label + "|" + str(y.shape[0]) + "|" +
                       ",".join(f"{v:.6f}" for v in y.tolist()) + "\n")
            print(f"[PROC] OK {label}: dim={y.shape[0]} L2={np.linalg.norm(y):.3f}")

    # --- Emit BIN per person ---
    emit_bin(out_bin, args.subject, vecs, encrypt_bytes)
    print(f"[DONE] Wrote:\n - {out_bin}\n - {out_txt}")

    # --- Emit PIN BIN if available ---
    emit_pin_bin(pin_txt, out_pin_bin, args.subject, encrypt_bytes)

    if args.save_npz:
        np.savez(out_npz, labels=np.array(labels),
                 embeddings=np.array(vecs, dtype=np.float32))
        print(f"[SAVE] NPZ saved: {out_npz}")

    return args.skip_auto


# ==============================
# Entrypoint (auto multi-person mode)
# ==============================
if __name__ == "__main__":
    try:
        skip_auto = main()
    except SystemExit as e:
        msg = str(e)
        if "No images found" in msg:
            print("\n[INFO] No images found in current folder — switching to multi-person mode.\n")
            skip_auto = False
        else:
            raise

    if not skip_auto:
        print("[AUTO] Multi-person mode activated.\n")
        root = os.path.dirname(os.path.abspath(__file__))
        model_path = os.path.join(root, "facenet_512_int_quant.tflite")

        subfolders = [
            f for f in os.listdir(root)
            if os.path.isdir(os.path.join(root, f))
            and not f.lower().startswith("output")
            and not f.startswith(".")
            and f != "all_faces"
            and any(glob.glob(os.path.join(root, f, ext)) for ext in ("*.jpg", "*.jpeg", "*.png", "*.bmp"))
        ]

        print(f"[AUTO] Found {len(subfolders)} people: {subfolders}\n")
        for name in subfolders:
            print(f"[AUTO] Generating BIN for {name} ...")
            cmd = [
                os.sys.executable, __file__,
                "--model", model_path,
                "--in_dir", os.path.join(root, name),
                "--subject", name,
                "--skip_auto",
            ]
            subprocess.run(cmd, check=True)

        all_faces_root = os.path.join(root, "all_faces")
        manifest = os.path.join(all_faces_root, "manifest.txt")
        bins = sorted(glob.glob(os.path.join(all_faces_root, "*", "*.bin")))
        with open(manifest, "w", encoding="utf-8") as f:
            for b in bins:
                f.write(os.path.relpath(b, all_faces_root).replace("\\", "/") + "\n")
        print(f"\n[AUTO] ✅ Manifest written: {manifest}")
