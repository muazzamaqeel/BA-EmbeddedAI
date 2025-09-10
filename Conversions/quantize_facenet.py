# quantize_keras_facenet.py
import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'  # quieter TF logs

import tensorflow as tf
import numpy as np
from pathlib import Path
from PIL import Image
from keras_facenet import FaceNet

print("[INFO] TF:", tf.__version__)

ROOT = Path(__file__).parent
OUT_FULL_INT8 = ROOT / "facenet_512_int_quant.tflite"
OUT_DR_INT8    = ROOT / "facenet_512_dynamic_quant.tflite"  # fallback
CALIB_DIR      = ROOT / "calib"

def representative_dataset():
    # Use real face crops if available, else fallback to neutral images
    imgs = []
    if CALIB_DIR.exists():
        imgs += list(CALIB_DIR.glob("*.jpg"))
        imgs += list(CALIB_DIR.glob("*.png"))
    if not imgs:
        print("[WARN] No calib images found; using dummy calibration frames.")
        for _ in range(50):
            arr = np.full((160,160,3), 127.5, dtype=np.float32)
            arr = (arr - 127.5) / 128.0       # FaceNet prewhiten
            yield [np.expand_dims(arr, 0)]
        return

    for p in imgs[:100]:
        try:
            img = Image.open(p).convert("RGB").resize((160,160))
            arr = np.asarray(img, dtype=np.float32)
            arr = (arr - 127.5) / 128.0       # FaceNet prewhiten
            yield [np.expand_dims(arr, 0)]
        except Exception as e:
            print(f"[WARN] Skipping {p.name}: {e}")

# Load Keras Facenet-512 (InceptionResNetV1, VGGFace2)
print("[INFO] Loading keras-facenet model…")
embedder = FaceNet()
model = embedder.model
model.trainable = False

# Concrete function for inference
infer = tf.function(lambda x: model(x, training=False),
                    input_signature=[tf.TensorSpec([None,160,160,3], tf.float32, name="input")])
concrete = infer.get_concrete_function()

# --- Try FULL int8 (best for STM32 NPU) ---
try:
    print("[INFO] Trying FULL int8 quantization (TFLITE_BUILTINS_INT8)…")
    conv = tf.lite.TFLiteConverter.from_concrete_functions([concrete])
    conv.optimizations = [tf.lite.Optimize.DEFAULT]
    conv.representative_dataset = representative_dataset
    conv.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    conv.inference_input_type  = tf.uint8
    conv.inference_output_type = tf.int8

    tfl = conv.convert()
    OUT_FULL_INT8.write_bytes(tfl)
    print(f"[OK] Wrote {OUT_FULL_INT8}  ({len(tfl):,} bytes)")

    # Quick IO sanity print
    interp = tf.lite.Interpreter(model_path=str(OUT_FULL_INT8))
    interp.allocate_tensors()
    print("[IO] input :", interp.get_input_details())
    print("[IO] output:", interp.get_output_details())
except Exception as e:
    print("[WARN] FULL int8 failed:", e)
    print("[INFO] Falling back to dynamic-range int8 (weights int8, IO float).")
    conv = tf.lite.TFLiteConverter.from_concrete_functions([concrete])
    conv.optimizations = [tf.lite.Optimize.DEFAULT]
    conv.representative_dataset = representative_dataset
    tfl = conv.convert()
    OUT_DR_INT8.write_bytes(tfl)
    print(f"[OK] Wrote {OUT_DR_INT8}  ({len(tfl):,} bytes)")

print("[DONE]")
