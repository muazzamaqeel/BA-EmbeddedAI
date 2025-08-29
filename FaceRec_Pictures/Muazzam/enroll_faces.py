import os
import cv2
import numpy as np
import tensorflow as tf

# --- Paths ---
BLAZEFACE_MODEL = r"C:\STM32_FaceRecognition\BA-EmbeddedAI\Model\blazeface_front_128_quant_pc_ff_od_wider_face.tflite"
FACEREC_MODEL   = r"C:\Model\mobilefacenet.tflite"
IMAGES_DIR      = r"C:\STM32_FaceRecognition\BA-EmbeddedAI\FaceRec_Pictures\Muazzam"
OUTPUT_EMB      = r"C:\STM32_FaceRecognition\BA-EmbeddedAI\FaceRec_Pictures\muazzam_embeddings.npy"
OUTPUT_CSV      = r"C:\STM32_FaceRecognition\BA-EmbeddedAI\FaceRec_Pictures\muazzam_embeddings.csv"

# --- Load TFLite interpreters ---
blazeface_interpreter = tf.lite.Interpreter(model_path=BLAZEFACE_MODEL)
blazeface_interpreter.allocate_tensors()
bf_input = blazeface_interpreter.get_input_details()
bf_output = blazeface_interpreter.get_output_details()

facerec_interpreter = tf.lite.Interpreter(model_path=FACEREC_MODEL)
facerec_interpreter.allocate_tensors()
fr_input = facerec_interpreter.get_input_details()[0]['index']
fr_output = facerec_interpreter.get_output_details()[0]['index']

def detect_face(image):
    """Run BlazeFace to get face bounding box"""
    h, w, _ = image.shape
    img_resized = cv2.resize(image, (128, 128))
    img_in = np.expand_dims(img_resized.astype(np.float32) / 255.0, axis=0)

    blazeface_interpreter.set_tensor(bf_input[0]['index'], img_in)
    blazeface_interpreter.invoke()

    boxes = blazeface_interpreter.get_tensor(bf_output[0]['index']).squeeze()  # (512,16)
    scores = blazeface_interpreter.get_tensor(bf_output[1]['index']).squeeze() # (512,)

    if scores.ndim > 1:
        scores = scores[:, 0]

    if len(scores) == 0:
        return None

    best_idx = int(np.argmax(scores))
    if scores[best_idx] < 0.5:
        return None

    # First 4 numbers are box [ymin, xmin, ymax, xmax] normalized
    ymin, xmin, ymax, xmax = boxes[best_idx][:4]

    # Convert to pixel coords
    x1 = max(0, int(xmin * w))
    y1 = max(0, int(ymin * h))
    x2 = min(w, int(xmax * w))
    y2 = min(h, int(ymax * h))

    if x2 <= x1 or y2 <= y1:
        return None

    return [x1, y1, x2, y2]



def get_embedding(face_img):
    """Run MobileFaceNet to get 128-D embedding"""
    face_resized = cv2.resize(face_img, (112, 112))
    face_norm = (face_resized.astype(np.float32) - 127.5) / 128.0
    input_tensor = np.expand_dims(face_norm, axis=0)

    facerec_interpreter.set_tensor(fr_input, input_tensor)
    facerec_interpreter.invoke()
    embedding = facerec_interpreter.get_tensor(fr_output)[0]
    return embedding / np.linalg.norm(embedding)

embeddings = []
for fname in os.listdir(IMAGES_DIR):
    if not fname.lower().endswith((".jpg", ".png")):
        continue
    path = os.path.join(IMAGES_DIR, fname)
    img = cv2.imread(path)
    bbox = detect_face(img)

    if bbox is None:
        print(f"[WARN] No face detected in {fname}")
        continue

    x1, y1, x2, y2 = bbox
    face_crop = img[y1:y2, x1:x2]

    emb = get_embedding(face_crop)
    embeddings.append(emb)
    print(f"[OK] Processed {fname}, embedding shape={emb.shape}")

embeddings = np.array(embeddings)
np.save(OUTPUT_EMB, embeddings)
np.savetxt(OUTPUT_CSV, embeddings, delimiter=",")
print(f"\n✅ Saved {len(embeddings)} embeddings to {OUTPUT_EMB} and {OUTPUT_CSV}")
