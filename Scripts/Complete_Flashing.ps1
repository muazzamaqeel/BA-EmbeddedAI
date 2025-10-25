# Complete_Flashing.ps1
# STM32N6570-DK: FSBL + Detector + FaceID + Main App (Final Script, Relative Paths, Persistent Console)
$ErrorActionPreference = 'Stop'

# --- External loader (absolute) ---
$env:DKEL = "C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\MX66UW1G45G_STM32N6570-DK.stldr"

# --- Base directory ---
$base = Split-Path -Parent $MyInvocation.MyCommand.Definition
$root = Join-Path $base ".."

# --- Relative paths ---
$FSBL   = Join-Path $root "Binary\ai_fsbl.hex"
$DETBIN = Join-Path $root "Model\network_atonbuf.xSPI2.bin"
$RECBIN = Join-Path $root "Model\FaceRecFiles\st_ai_output\face_recognition_data.bin"
$APPHEX = Join-Path $root "STM32CubeIDE\STM32N6570-DK\Debug\x-cube-n6-ai-people-detection-tracking-dk.hex"

# --- Check files exist ---
foreach ($file in @($FSBL, $DETBIN, $RECBIN, $APPHEX)) {
    if (-not (Test-Path $file)) {
        throw "Missing required file: $file"
    }
}

Write-Host ""
Write-Host "============================================="
Write-Host " STM32N6570-DK FULL FLASH SCRIPT (FSBL + DET + FACEID + APP)"
Write-Host "============================================="
Write-Host ""

# --- 1) FSBL ---
Write-Host "Flashing FSBL..."
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el "$env:DKEL" -hardRst -w "$FSBL" -v
if ($LASTEXITCODE -ne 0) { throw "FSBL flash failed!" }
Start-Sleep -Seconds 2

# --- 2) Face Detection model ---
Write-Host "Flashing Face Detection model @ 0x71000000..."
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el "$env:DKEL" -w "$DETBIN" 0x71000000 -v
if ($LASTEXITCODE -ne 0) { throw "Detector model flash failed!" }
Start-Sleep -Seconds 1

# --- 3) Face Recognition model ---
Write-Host "Flashing Face Recognition model @ 0x71040000..."
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el "$env:DKEL" -w "$RECBIN" 0x71040000 -v
if ($LASTEXITCODE -ne 0) { throw "FaceID model flash failed!" }
Start-Sleep -Seconds 1

# --- 4) Main firmware ---
Write-Host "Flashing main firmware..."
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el "$env:DKEL" -hardRst -w "$APPHEX" -v
if ($LASTEXITCODE -ne 0) { throw "Application firmware flash failed!" }

# --- 5) Verification ---
Write-Host "Verifying detector and FaceID headers..."
STM32_Programmer_CLI -c port=SWD mode=UR -el "$env:DKEL" -r32 0x71000000 16
STM32_Programmer_CLI -c port=SWD mode=UR -el "$env:DKEL" -r32 0x71040000 16

# --- 6) Full compare for FaceID ---
Write-Host "Reading back FaceID region for byte-compare..."
$size = (Get-Item "$RECBIN").Length
STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el "$env:DKEL" -u 0x71040000 $size "$base\readback_faceid.bin"
fc.exe /b "$base\readback_faceid.bin" "$RECBIN"

Write-Host ""
Write-Host "All flash operations completed successfully!"
Write-Host "Press ENTER to close this window..."
Read-Host
