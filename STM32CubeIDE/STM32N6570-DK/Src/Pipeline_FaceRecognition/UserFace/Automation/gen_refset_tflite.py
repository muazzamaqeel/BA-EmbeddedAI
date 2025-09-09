# gen_refset_tflite.py
import argparse, os, glob, pathlib
import numpy as np
from PIL import Image

# Use tflite-runtime if available; otherwise fall back to TensorFlow's lite interpreter.
try:
    import tflite_runtime.interpreter as tflite
    _TFLITE = True
except Exception:
    import tensorflow as tf  # requires "pip install tensorflow"
    _TFLITE = False


def center_square_resize_to_minus1_1(path, size=(160, 160)):
    """Load image, center-square crop, resize, return HWC float32 in [-1, 1]."""
    im = Image.open(path).convert("RGB")
    w, h = im.size
    side = min(w, h)
    x0 = (w - side) // 2
    y0 = (h - side) // 2
    im = im.crop((x0, y0, x0 + side, y0 + side))
    im = im.resize(size, Image.BILINEAR)
    arr = np.asarray(im, dtype=np.float32)  # HWC, [0..255]
    return arr * (1.0 / 127.5) - 1.0        # [-1, 1]


def quantize_input(x_minus1_1_hwc, in_scale, in_zp, in_dtype, input_shape):
    """
    Quantize [-1,1] HWC float -> model's quantized dtype (usually uint8, zp=128, scale≈2/255)
    and reshape to model's expected layout (NHWC or NCHW) based on input_shape.
    """
    if in_dtype != np.uint8:
        raise SystemExit(f"Model input dtype {in_dtype} not supported (expected uint8).")

    # q = round(x/scale + zp)
    q = np.round(x_minus1_1_hwc / in_scale + in_zp).astype(np.int32)
    q = np.clip(q, 0, 255).astype(np.uint8)  # HWC uint8

    # Determine layout from input_shape (e.g. [1,160,160,3] or [1,3,160,160])
    if len(input_shape) != 4:
        raise SystemExit(f"Unexpected input rank: {input_shape}")

    n, d1, d2, d3 = input_shape[0], input_shape[1], input_shape[2], input_shape[3]
    if d3 == 3:
        # NHWC
        q = q[None, ...]  # [1,H,W,C]
    elif d1 == 3:
        # NCHW
        q = np.transpose(q, (2, 0, 1))  # CHW
        q = q[None, ...]                # [1,C,H,W]
    else:
        raise SystemExit(f"Cannot infer layout from shape {input_shape}")
    return q


def dequantize_output(y_q, out_scale, out_zp, out_dtype):
    """int8 (usually) -> float32 using model's quantization parameters."""
    if out_dtype != np.int8:
        raise SystemExit(f"Model output dtype {out_dtype} not supported (expected int8).")
    return (y_q.astype(np.float32) - out_zp) * out_scale


def emit_c_struct(out_c, names, vecs):
    with open(out_c, "w", encoding="utf-8") as f:
        f.write('#include <stdint.h>\n')
        f.write('typedef struct { const char* name; const float* data; int dim; } EmbRec;\n')
        for i, (nm, v) in enumerate(zip(names, vecs)):
            sym = f"emb_{i}"
            f.write(f"static const float {sym}[] = {{\n")
            for k, val in enumerate(v):
                end = ",\n" if (k + 1) % 8 == 0 else ", "
                if k + 1 == len(v): end = "\n"
                f.write(f"  {val:.8f}{end}")
            f.write("};\n")
        f.write("\nconst EmbRec g_ref_set[] = {\n")
        for i, nm in enumerate(names):
            f.write(f'  {{ "{nm}", emb_{i}, (int)(sizeof(emb_{i})/sizeof(float)) }},\n')
        f.write("};\n")
        f.write("const int g_ref_set_count = (int)(sizeof(g_ref_set)/sizeof(g_ref_set[0]));\n")


def main():
    ap = argparse.ArgumentParser(description="Generate embeddings & C table from a quantized TFLite face-recognition model.")
    ap.add_argument("--model", required=True, help="Path to TFLite model (e.g., facenet_512_int_quant.tflite)")
    ap.add_argument("--in_dir", required=True, help="Folder with images (bmp/png/jpg)")
    ap.add_argument("--size", type=int, default=160, help="Square input size (default 160)")
    ap.add_argument("--l2norm", action="store_true", help="Apply L2 normalization to output embeddings")
    ap.add_argument("--out_txt", default="embeddings.txt", help="Write embeddings lines here")
    ap.add_argument("--emit_c", required=True, help="Write C table here (embeddings_table.c)")
    args = ap.parse_args()

    # Load interpreter
    if _TFLITE:
        interpreter = tflite.Interpreter(model_path=args.model)
    else:
        interpreter = tf.lite.Interpreter(model_path=args.model)
    interpreter.allocate_tensors()

    in_det = interpreter.get_input_details()[0]
    out_det = interpreter.get_output_details()[0]

    in_idx  = in_det["index"]
    out_idx = out_det["index"]
    in_q    = in_det.get("quantization", (0.0, 0))
    out_q   = out_det.get("quantization", (0.0, 0))
    in_scale, in_zp   = (float(in_q[0]), int(in_q[1]))
    out_scale, out_zp = (float(out_q[0]), int(out_q[1]))
    in_dtype, out_dtype = (in_det["dtype"], out_det["dtype"])
    in_shape = list(in_det["shape"])

    # Collect images
    exts = ("*.bmp", "*.png", "*.jpg", "*.jpeg")
    files = sorted(sum([glob.glob(os.path.join(args.in_dir, e)) for e in exts], []))
    if not files:
        raise SystemExit(f"No images found in {args.in_dir}")

    names, vecs = [], []
    with open(args.out_txt, "w", encoding="utf-8") as outf:
        for fp in files:
            x = center_square_resize_to_minus1_1(fp, size=(args.size, args.size))  # HWC [-1,1]
            q = quantize_input(x, in_scale, in_zp, in_dtype, in_shape)             # NHWC or NCHW per model

            interpreter.set_tensor(in_idx, q)
            interpreter.invoke()
            y_q = interpreter.get_tensor(out_idx).reshape(-1)                      # int8
            y = dequantize_output(y_q, out_scale, out_zp, out_dtype)               # float32

            if args.l2norm:
                n = np.linalg.norm(y) + 1e-6
                y = y / n

            label = pathlib.Path(fp).stem
            dim = y.shape[0]
            line = label + "|" + str(dim) + "|" + ",".join(f"{v:.6f}" for v in y.tolist())
            print(line)
            outf.write(line + "\n")

            names.append(label)
            vecs.append(y.tolist())

    emit_c_struct(args.emit_c, names, vecs)
    print("Wrote:", args.emit_c)
    print(f"Quant (input):  scale={in_scale}  zp={in_zp}  dtype={in_dtype}  shape={in_shape}")
    print(f"Quant (output): scale={out_scale} zp={out_zp} dtype={out_dtype}")


if __name__ == "__main__":
    main()
