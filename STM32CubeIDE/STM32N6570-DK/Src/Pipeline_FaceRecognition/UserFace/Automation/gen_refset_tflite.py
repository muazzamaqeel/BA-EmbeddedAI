#!/usr/bin/env python3
import argparse, os, glob, pathlib, subprocess
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

def encrypt_bytes(data: bytes) -> bytes:
    """Encrypt raw bytes using AES-CBC (no padding if already 16-byte aligned)."""
    print(f"[AES] Raw length: {len(data)} bytes")
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    if len(data) % 16 == 0:
        print("[AES] Length is block-aligned, skipping padding")
        enc = cipher.encrypt(data)
    else:
        print("[AES] Length not aligned, padding applied")
        enc = cipher.encrypt(pad(data, 16))
    print(f"[AES] Encrypted length: {len(enc)} bytes")
    print(f"[AES] First 16 bytes ciphertext: {enc[:16].hex().upper()}")
    return enc


# ==============================
# TensorFlow Lite import (prefer tflite-runtime)
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
    arr = arr * (1.0 / 127.5) - 1.0
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
# C Code Generation with Encryption
# ==============================
def emit_c_struct(out_c, subject, vecs):
    print(f"[CGEN] Writing encrypted C file → {out_c}")
    dim = len(vecs[0])
    safe_subject = subject.replace("-", "_").replace(" ", "_")

    with open(combined_path, "w", encoding="utf-8") as f:
        f.write("/* Auto-generated merged reference set — DO NOT EDIT */\n")
        f.write("#include <stddef.h>\n")
        f.write("#include <stdint.h>\n")  # <-- Added to fix uint8_t type
        f.write("#ifndef EMBREC_DEFINED\n#define EMBREC_DEFINED\n")
        f.write("typedef struct { const char* name; const uint8_t* data; int dim; } EmbRec;\n")
        f.write("#endif\n\n")


        f.write(f"#define REF_NAME \"{subject}\"\n\n")

        for i, v in enumerate(vecs):
            raw = np.array(v, dtype=np.float32).tobytes()
            print(f"[CGEN] Encrypting embedding {i}: raw len={len(raw)}")
            enc = encrypt_bytes(raw)
            print(f"[CGEN] -> ciphertext len={len(enc)}")
            f.write(f"static const uint8_t emb_{safe_subject}_{i}[] = {{\n  ")
            for k, val in enumerate(enc):
                sep = ", " if (k + 1) % 16 else ",\n  "
                if k + 1 == len(enc):
                    sep = "\n"
                f.write(f"0x{val:02X}{sep}")
            f.write("};\n\n")

        f.write(f"const EmbRec g_ref_set_{safe_subject}[] = {{\n")
        for i in range(len(vecs)):
            f.write(f"  {{ REF_NAME, emb_{safe_subject}_{i}, FR_EMB_SIZE }},\n")
        f.write("};\n")
        f.write(f"const int g_ref_set_count_{safe_subject} = "
                f"(int)(sizeof(g_ref_set_{safe_subject})/sizeof(g_ref_set_{safe_subject}[0]));\n\n")
        f.write("#undef REF_NAME\n")

    print(f"[CGEN] ✅ Finished writing {len(vecs)} embeddings to {out_c}")


# ==============================
# Main Function
# ==============================
def main():
    print("\n=== Face Embedding Encryption Tool ===")
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default="facenet_512_int_quant.tflite")
    ap.add_argument("--in_dir", default=".")
    ap.add_argument("--subject", default="Muazzam")
    ap.add_argument("--no_l2norm", action="store_true")
    ap.add_argument("--save_npz", action="store_true")
    ap.add_argument("--skip_auto", action="store_true")
    args = ap.parse_args()

    root = os.path.dirname(os.path.abspath(__file__))
    all_faces_root = os.path.join(root, "all_faces")
    os.makedirs(all_faces_root, exist_ok=True)
    person_dir = os.path.join(all_faces_root, args.subject)
    os.makedirs(person_dir, exist_ok=True)

    out_txt = os.path.join(person_dir, f"embeddings_{args.subject}.txt")
    out_c = os.path.join(person_dir, f"embeddings_table_{args.subject}.c")
    out_npz = os.path.join(person_dir, f"embeddings_{args.subject}.npz")

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

    emit_c_struct(out_c, args.subject, vecs)
    print(f"[DONE] Wrote:\n - {out_c}\n - {out_txt}")
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
            print(f"[AUTO] Generating embeddings for {name} ...")
            cmd = [
                os.sys.executable, __file__,
                "--model", model_path,
                "--in_dir", os.path.join(root, name),
                "--subject", name,
                "--skip_auto",
            ]
            subprocess.run(cmd, check=True)

        combined_path = os.path.join(root, "all_faces", "combined_refset.c")
        print(f"\n[AUTO] Building {combined_path}")
        with open(combined_path, "w", encoding="utf-8") as f:
            f.write("/* Auto-generated merged reference set — DO NOT EDIT */\n")
            f.write("#include <stddef.h>\n")
            f.write("#ifndef EMBREC_DEFINED\n#define EMBREC_DEFINED\n")
            f.write("typedef struct { const char* name; const uint8_t* data; int dim; } EmbRec;\n")
            f.write("#endif\n\n")
            for name in subfolders:
                f.write(f"#include \"embeddings_table_{name}.c\"\n")

            f.write("\nconst EmbRec* g_all_ref_sets[] = {\n")
            for name in subfolders:
                f.write(f"    g_ref_set_{name},\n")
            f.write("};\n\nconst int g_all_ref_set_counts[] = {\n")
            for name in subfolders:
                f.write(f"    g_ref_set_count_{name},\n")
            f.write("};\n\nstatic EmbRec g_ref_set_combined[512];\n")
            f.write("static int g_ref_set_combined_count = 0;\n")
            f.write("EmbRec* g_ref_set = g_ref_set_combined;\nint g_ref_set_count = 0;\n\n")
            f.write("void FR_BuildCombinedRefset(void) {\n")
            f.write("    g_ref_set_combined_count = 0;\n    g_ref_set_count = 0;\n")
            f.write("    int total = sizeof(g_all_ref_set_counts)/sizeof(int);\n")
            f.write("    for (int i = 0; i < total; i++) {\n")
            f.write("        int cnt = g_all_ref_set_counts[i];\n")
            f.write("        const EmbRec* sub = g_all_ref_sets[i];\n")
            f.write("        for (int j = 0; j < cnt && g_ref_set_combined_count < 512; j++) {\n")
            f.write("            g_ref_set_combined[g_ref_set_combined_count++] = sub[j];\n")
            f.write("        }\n    }\n")
            f.write("    g_ref_set = g_ref_set_combined;\n")
            f.write("    g_ref_set_count = g_ref_set_combined_count;\n}\n")
        print(f"[AUTO] ✅ Combined file written: {combined_path}\n")
