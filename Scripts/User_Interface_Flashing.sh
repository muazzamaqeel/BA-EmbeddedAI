#!/bin/bash
# ================================================================
# STM32N6570-DK — UI Image Flashing Utility (Bash version, fixed)
# ================================================================

# --- Always use GNU find (avoid Windows find.exe issues) ---
export PATH="/usr/bin:/bin:$PATH"

set -e  # Stop on first error

# --- Base directories ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_DIR="$(realpath "$SCRIPT_DIR/../STM32CubeIDE/STM32N6570-DK/Src/App_UI/Resources")"
STM32Prog="STM32_Programmer_CLI"

# --- Load external loader path from config.json ---
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

# --- Flash address map ---
declare -A FlashMap=(
    ["2_Start"]="0x77E00000"
    ["3_admin"]="0x778A0000"
    ["pin"]="0x77AE0000"
)

# --- Function: Convert and Flash ---
convert_and_flash() {
    local dirName="$1"
    local flashAddr="$2"
    local folder="$BASE_DIR/$dirName"

    if [ ! -d "$folder" ]; then
        echo "⚠️  Skipping $dirName (folder not found)"
        return
    fi

    echo ""
    echo "=== Processing $dirName ==="

    # 1) Existing .bin file
    local binFile
    binFile=$(/usr/bin/find "$folder" -maxdepth 1 -type f -iregex ".*\.bin$" | head -n 1)
    if [ -n "$binFile" ]; then
        echo "Found existing .bin file: $(basename "$binFile")"
        echo "Flashing $(basename "$binFile") to $flashAddr..."
        if $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -w "$binFile" "$flashAddr" -v; then
            echo "Flashed $dirName at $flashAddr"
        else
            echo "Flash failed for $dirName"
        fi
        return
    fi

    # 2) Otherwise find image (.bmp/.jpg/.png)
    local image
    image=$(/usr/bin/find "$folder" -maxdepth 1 -type f -iregex ".*\.\(bmp\|jpg\|jpeg\|png\)$" | head -n 1)
    if [ -z "$image" ]; then
        echo "No .bmp/.jpg/.png/.bin found in $dirName"
        return
    fi

    local baseName
    baseName=$(basename "$image")
    baseName="${baseName%.*}"
    local resizedBMP="$folder/${baseName}_stm32.bmp"
    local binFilePath="$folder/${baseName}.bin"

    echo "Converting $(basename "$image") → $(basename "$resizedBMP")..."
    if ! magick "$image" -resize 800x480! -depth 8 -type truecolor -compress none BMP3:"$resizedBMP"; then
        echo "ImageMagick conversion failed for $dirName"
        return
    fi

    if [ ! -f "$resizedBMP" ]; then
        echo "Conversion failed: $resizedBMP not created"
        return
    fi

    cp -f "$resizedBMP" "$binFilePath"
    echo "Flashing $(basename "$binFilePath") to $flashAddr..."
    if $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -w "$binFilePath" "$flashAddr" -v; then
        echo "Flashed $dirName at $flashAddr"
    else
        echo "Flash failed for $dirName"
    fi
}

# --- Main execution ---
echo "============================================="
echo " STM32N6570-DK UI Image Flashing Utility"
echo "============================================="

for key in "${!FlashMap[@]}"; do
    convert_and_flash "$key" "${FlashMap[$key]}"
done

echo ""
echo "All flashing operations completed."
read -rp "Press ENTER to close this window..."
