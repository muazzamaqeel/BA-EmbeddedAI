#!/bin/bash
# ================================================================
# STM32N6570-DK — Full NOR Flash Memory Cleanup (Encoding-Safe)
# ================================================================

set -e  # Stop on any error

STM32Prog="STM32_Programmer_CLI"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_PATH="$SCRIPT_DIR/config.json"

# --- Check config file ---
if [ ! -f "$CONFIG_PATH" ]; then
    echo "Error: Config file not found: $CONFIG_PATH"
    exit 1
fi

# --- Parse LoaderPath from JSON ---
Loader=$(grep -oP '"LoaderPath"\s*:\s*"\K[^"]+' "$CONFIG_PATH" || true)
if [ -z "$Loader" ]; then
    echo "Error: Failed to read config.json. Ensure it's valid JSON."
    exit 1
fi

clear
echo ""
echo " STM32N6570-DK — FULL NOR MEMORY CLEANUP"
echo ""

# --- Check if STM32_Programmer_CLI exists ---
if ! command -v "$STM32Prog" &> /dev/null; then
    echo "Error: STM32_Programmer_CLI not found. Install STM32CubeProgrammer and retry."
    echo ""
    read -p "Press ENTER to close this window"
    exit 1
fi

echo ""
version=$($STM32Prog --version)
echo "STM32CubeProgrammer detected: $version"
echo ""
echo "Starting full external NOR erase..."
echo "This may take several minutes. Do not disconnect power or ST-LINK."
echo ""

# --- Perform full erase ---
if ! $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -e all; then
    echo "Error: NOR flash erase failed."
    echo ""
    read -p "Press ENTER to close this window"
    exit 1
fi

echo ""
echo "Verifying NOR memory (first 16 words at 0x70000000)..."
$STM32Prog -c port=SWD mode=UR -el "$Loader" -r32 0x70000000 16

echo ""
echo "=============================================================="
echo "   COMPLETE NOR MEMORY ERASE FINISHED SUCCESSFULLY"
echo "=============================================================="
echo ""
read -p "Press ENTER to close this window"
