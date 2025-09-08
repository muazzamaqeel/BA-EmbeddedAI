import argparse, os, sys, glob, pathlib
import numpy as np
from PIL import Image
import onnxruntime as ort

def load_and_preprocess(p, size=(112,112), layout="nchw"):
    im = Image.open(p).convert("RGB")
    # center-square crop
    w, h = im.size
    side = min(w, h)
    x0 = (w - side)//2
    y0 = (h - side)//2
    im = im.crop((x0, y0, x0+side, y0+side))
    im = im.resize(size, Image.BILINEAR)
    arr = np.asarray(im, dtype=np.float32)  # HWC, [0..255]
    # [-1, 1] like firmware
    arr = arr*(1.0/127.5) - 1.0
    if layout.lower() == "nchw":
        arr = np.transpose(arr, (2,0,1))  # CHW
        arr = arr[None, ...]              # NCHW
    else:
        arr = arr[None, ...]              # NHWC
    return arr

def emit_c_struct(out_c, names, vecs):
    with open(out_c, "w", encoding="utf-8") as f:
        f.write('#include <stdint.h>\n')
        f.write('typedef struct { const char* name; const float* data; int dim; } EmbRec;\n')
        for i,(nm, v) in enumerate(zip(names, vecs)):
            sym = f"emb_{i}"
            f.write(f"static const float {sym}[] = {{\n")
            for k,val in enumerate(v):
                end = ",\n" if (k+1)%8==0 else ", "
                if k+1 == len(v): end = "\n"
                f.write(f"  {val:.8f}{end}")
            f.write("};\n")
        f.write("\nconst EmbRec g_ref_set[] = {\n")
        for i,nm in enumerate(names):
            f.write(f'  {{ "{nm}", emb_{i}, (int)(sizeof(emb_{i})/sizeof(float)) }},\n')
        f.write("};\n")
        f.write("const int g_ref_set_count = (int)(sizeof(g_ref_set)/sizeof(g_ref_set[0]));\n")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", required=True, help="Path to MobileFaceNet ONNX (e.g., mobilefacenet_int8_faces.onnx)")
    ap.add_argument("--in_dir", required=True, help="Folder with images (bmp/png/jpg)")
    ap.add_argument("--layout", default="nchw", choices=["nchw","nhwc"], help="Model input layout (default nchw)")
    ap.add_argument("--l2norm", action="store_true", help="Apply L2 normalization to output embeddings")
    ap.add_argument("--out_txt", default="embeddings.txt", help="Where to write text lines")
    ap.add_argument("--emit_c", default=None, help="Optional: write a C file with embeddings table")
    args = ap.parse_args()

    # ONNX Runtime session
    sess = ort.InferenceSession(args.model, providers=["CPUExecutionProvider"])
    in_name  = sess.get_inputs()[0].name
    out_name = sess.get_outputs()[0].name

    # Gather images
    exts = ("*.bmp","*.png","*.jpg","*.jpeg")
    files = []
    for e in exts:
        files.extend(glob.glob(os.path.join(args.in_dir, e)))
    files = sorted(files)
    if not files:
        print("No images found in", args.in_dir, file=sys.stderr)
        sys.exit(1)

    names = []
    vecs  = []

    with open(args.out_txt, "w", encoding="utf-8") as outf:
        for fp in files:
            arr = load_and_preprocess(fp, layout=args.layout)
            y = sess.run([out_name], {in_name: arr})[0]
            y = np.asarray(y).reshape(-1).astype(np.float32)

            if args.l2norm:
                n = np.linalg.norm(y) + 1e-6
                y = y / n

            label = pathlib.Path(fp).stem
            dim = y.shape[0]
            # print console-style line
            line = label + "|" + str(dim) + "|" + ",".join(f"{v:.6f}" for v in y.tolist())
            print(line)
            outf.write(line + "\n")

            names.append(label)
            vecs.append(y.tolist())

    if args.emit_c:
        emit_c_struct(args.emit_c, names, vecs)
        print("Wrote C embeddings table to", args.emit_c)

if __name__ == "__main__":
    main()
