#!/bin/bash
# ================================================================
# STM32N6570-DK — UI Image Flashing Utility (Bash version, fixed)
# ================================================================

# Use GNU find instead of Windows find.exe
export PATH="/usr/bin:/bin:$PATH"

set -e  # Stop on first error

# --- Base directories ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_DIR_UNIX="$(realpath "$SCRIPT_DIR/../STM32CubeIDE/STM32N6570-DK/Src/UI/Resources")"

# Convert BASE_DIR to Windows-native path for ImageMagick
BASE_DIR="$(cygpath -w "$BASE_DIR_UNIX")"

STM32Prog="STM32_Programmer_CLI"

# --- Load external loader path from config.json ---
CONFIG_PATH="$SCRIPT_DIR/config.json"
if [ ! -f "$CONFIG_PATH" ]; then
    echo "Error: Missing configuration file: $CONFIG_PATH"
    exit 1
fi

Loader=$(grep -oP '"LoaderPath"\s*:\s*"\K[^"]+' "$CONFIG_PATH" || true)
if [ -z "$Loader" ]; then
    echo "Error: Failed to parse LoaderPath from config.json"
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

# -------------------------------------------------------------------
# Function: Convert and Flash
# -------------------------------------------------------------------
convert_and_flash() {
    local dirName="$1"
    local flashAddr="$2"

    local folder_unix="$BASE_DIR_UNIX/$dirName"
    local folder_win="$BASE_DIR\\$dirName"

    if [ ! -d "$folder_unix" ]; then
        echo "⚠️  Skipping $dirName (folder not found)"
        return
    fi

    echo ""
    echo "=== Processing $dirName ==="

    # Search for existing .bin first (Unix path)
    local binFile
    binFile=$(/usr/bin/find "$folder_unix" -maxdepth 1 -type f -iname "*.bin" | head -n 1)

    if [ -n "$binFile" ]; then
        local binFileWin
        binFileWin=$(cygpath -w "$binFile")

        echo "Found existing .bin file: $(basename "$binFile")"
        echo "Flashing to $flashAddr..."
        $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -w "$binFileWin" "$flashAddr" -v
        echo "Flashed $dirName at $flashAddr"
        return
    fi

    # Otherwise find image
    local image
    image=$(/usr/bin/find "$folder_unix" -maxdepth 1 -type f -iregex ".*\.\(bmp\|jpg\|jpeg\|png\)$" | head -n 1)

    if [ -z "$image" ]; then
        echo "❌ No .bmp/.jpg/.png/.bin found inside $dirName"
        return
    fi

    local baseName
    baseName=$(basename "$image")
    baseName="${baseName%.*}"

    # Output files (Unix + Windows versions)
    local resizedBMP_unix="$folder_unix/${baseName}_stm32.bmp"
    local resizedBMP_win
    resizedBMP_win=$(cygpath -w "$resizedBMP_unix")

    local binFile_unix="$folder_unix/${baseName}.bin"
    local binFile_win
    binFile_win=$(cygpath -w "$binFile_unix")

    echo "Converting $(basename "$image") → ${baseName}_stm32.bmp..."

    # Ensure directory exists
    mkdir -p "$folder_unix"

    # Convert using Windows paths for ImageMagick
    local image_win
    image_win=$(cygpath -w "$image")

    if ! magick "$image_win" -resize 800x480! -depth 8 -type truecolor -compress none BMP3:"$resizedBMP_win"; then
        echo "❌ ImageMagick conversion failed for $dirName"
        return
    fi

    if [ ! -f "$resizedBMP_unix" ]; then
        echo "❌ Conversion failed: $resizedBMP_unix not created"
        return
    fi

    cp -f "$resizedBMP_unix" "$binFile_unix"

    echo "Flashing ${baseName}.bin to $flashAddr..."
    $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -w "$binFile_win" "$flashAddr" -v
    echo "Flashed $dirName at $flashAddr"
}

# -------------------------------------------------------------------
# Main Execution
# -------------------------------------------------------------------
echo "============================================="
echo " STM32N6570-DK UI Image Flashing Utility"
echo "============================================="

for key in "${!FlashMap[@]}"; do
    convert_and_flash "$key" "${FlashMap[$key]}"
done

echo ""
echo "All flashing operations completed."
read -rp "Press ENTER to close this window..."
