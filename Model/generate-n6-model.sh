#!/bin/bash
set -e

# Run this script from repo root OR from Model/ (it works either way)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

MODEL_DIR="$REPO_ROOT/Model"
OUT_DIR="$MODEL_DIR/st_ai_output_blazeface"   # keep outputs separate/clean
BINARY_DIR="$REPO_ROOT/Binary"

MODEL_TFLITE="$MODEL_DIR/blazeface_front_128_quant_pc_ff_od_wider_face.tflite"
NEURAL_ART_JSON="$MODEL_DIR/user_neuralart.json"

# DK demo’s xSPI2 base (works for this project). Change only if your tool says otherwise.
XSPI2_BASE="0x70380000"

mkdir -p "$OUT_DIR" "$BINARY_DIR"

echo "[1/4] Generate BlazeFace for STM32N6 (xSPI2 weights)…"
stedgeai generate \
  --no-inputs-allocation --no-outputs-allocation \
  --model "$MODEL_TFLITE" \
  --target stm32n6 \
  --st-neural-art "default@$NEURAL_ART_JSON" \
  --output-dir "$OUT_DIR"

echo "[2/4] Copy network interfaces to Model/"
cp "$OUT_DIR/network.c"         "$MODEL_DIR/"
cp "$OUT_DIR/network.h"         "$MODEL_DIR/"
cp "$OUT_DIR/network_ecblobs.h" "$MODEL_DIR/"

echo "[3/4] Build DK weights pack (xSPI2 -> Intel HEX @ $XSPI2_BASE)…"
RAW="$OUT_DIR/network_atonbuf.xSPI2.raw"
BIN="$MODEL_DIR/network_data.xSPI2.bin"
HEX="$BINARY_DIR/network_data-dk.hex"

if [ ! -f "$RAW" ]; then
  echo "ERROR: $RAW not found. Check stedgeai output." >&2
  exit 1
fi

cp "$RAW" "$BIN"
arm-none-eabi-objcopy -I binary "$BIN" --change-addresses "$XSPI2_BASE" -O ihex "$HEX"

echo "[4/4] Done."
echo "  -> Model:  $MODEL_DIR/network.c, network.h, network_ecblobs.h"
echo "  -> DK hex: $HEX"
