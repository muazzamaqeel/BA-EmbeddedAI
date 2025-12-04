#!/bin/bash
# ================================================================
# STM32N6570-DK — FSBL + Detector + FaceID + Main App
# (Final Script, Relative Paths, Persistent Console)
# ================================================================

set -e  # Stop on first error

# --- Programmer path (LINUX FIX) ---
STM32_Programmer_CLI="/home/c24-02-student/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"

# --- External loader (from config.json) ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_PATH="$SCRIPT_DIR/config.json"

if [ ! -f "$CONFIG_PATH" ]; then
    echo "Error: Missing configuration file: $CONFIG_PATH"
    exit 1
fi

Loader=$(grep -oP '"LoaderPath"\s*:\s*"\K[^"]+' "$CONFIG_PATH" || true)
if [ -z "$Loader" ]; then
    echo "Error: Failed to parse config.json — check syntax or file path."
    exit 1
fi

if [ ! -f "$Loader" ]; then
    echo "Error: Invalid loader path: $Loader"
    exit 1
fi

echo "Using External Loader: $Loader"
echo ""

# --- Base directory ---
BASE_DIR="$SCRIPT_DIR"
ROOT_DIR="$(realpath "$BASE_DIR/..")"

# --- Relative paths ---
FSBL="$ROOT_DIR/Binary/ai_fsbl.hex"
DETBIN="$ROOT_DIR/Model/network_atonbuf.xSPI2.bin"
RECBIN="$ROOT_DIR/Model/FaceRecFiles/st_ai_output/face_recognition_data.bin"
APPHEX="$ROOT_DIR/STM32CubeIDE/STM32N6570-DK/Debug/x-cube-n6-ai-people-detection-tracking-dk.hex"

# --- Check files exist ---
for file in "$FSBL" "$DETBIN" "$RECBIN" "$APPHEX"; do
    if [ ! -f "$file" ]; then
        echo "Error: Missing required file: $file"
        exit 1
    fi
done

echo ""
echo "============================================="
echo " STM32N6570-DK FULL FLASH SCRIPT (FSBL + DET + FACEID + APP)"
echo "============================================="
echo ""

# --- 1) FSBL ---
echo "Flashing FSBL..."
if ! "$STM32_Programmer_CLI" -c port=SWD mode=HOTPLUG -el "$Loader" -hardRst -w "$FSBL" -v; then
    echo "Error: FSBL flash failed!"
    exit 1
fi
sleep 2

# --- 2) Face Detection model ---
echo "Flashing Face Detection model @ 0x71000000..."
if ! "$STM32_Programmer_CLI" -c port=SWD mode=HOTPLUG -el "$Loader" -w "$DETBIN" 0x71000000 -v; then
    echo "Error: Detector model flash failed!"
    exit 1
fi
sleep 1

# --- 3) Face Recognition model ---
echo "Flashing Face Recognition model @ 0x71040000..."
if ! "$STM32_Programmer_CLI" -c port=SWD mode=HOTPLUG -el "$Loader" -w "$RECBIN" 0x71040000 -v; then
    echo "Error: FaceID model flash failed!"
    exit 1
fi
sleep 1

# --- 4) Main firmware ---
echo "Flashing main firmware..."
if ! "$STM32_Programmer_CLI" -c port=SWD mode=HOTPLUG -el "$Loader" -hardRst -w "$APPHEX" -v; then
    echo "Error: Application firmware flash failed!"
    exit 1
fi

# --- 5) Verification ---
echo "Verifying detector and FaceID headers..."
"$STM32_Programmer_CLI" -c port=SWD mode=UR -el "$Loader" -r32 0x71000000 16
"$STM32_Programmer_CLI" -c port=SWD mode=UR -el "$Loader" -r32 0x71040000 16

# --- 6) Full compare for FaceID ---
echo "Reading back FaceID region for byte-compare..."
size=$(stat -c%s "$RECBIN")
"$STM32_Programmer_CLI" -c port=SWD mode=HOTPLUG -el "$Loader" -u 0x71040000 "$size" "$BASE_DIR/readback_faceid.bin"

echo ""
echo "Comparing FaceID region with original..."
if cmp -s "$BASE_DIR/readback_faceid.bin" "$RECBIN"; then
    echo "Binary compare: OK"
else
    echo "Binary compare: MISMATCH!"
    exit 1
fi

echo ""
echo "All flash operations completed successfully!"
read -rp "Press ENTER to close this window..."
