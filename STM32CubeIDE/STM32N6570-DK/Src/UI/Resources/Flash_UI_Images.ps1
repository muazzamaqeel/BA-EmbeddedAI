param()

# --- Configuration ---
$BaseDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$STM32Prog = "STM32_Programmer_CLI"
$Loader = "C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\MX66UW1G45G_STM32N6570-DK.stldr"

# Map of folders → flash addresses
$FlashMap = @{
    "2_Start" = "0x77E00000"
    "3_admin" = "0x778A0000"
    "pin"     = "0x77AE0000"
}

function Convert-And-Flash($DirName, $FlashAddr) {
    $Folder = Join-Path $BaseDir $DirName
    if (-not (Test-Path $Folder)) {
        Write-Warning "Skipping $DirName (folder not found)"
        return
    }

    Write-Host "`n=== Processing $DirName ===" -ForegroundColor Cyan

    # 1️⃣ Check for .bin first
    $BinFile = Get-ChildItem -Path $Folder -File | Where-Object { $_.Extension -match '(?i)\.bin$' } | Select-Object -First 1
    if ($BinFile) {
        Write-Host "Found existing .bin file: $($BinFile.Name)"
        Write-Host "Flashing $($BinFile.Name) to $FlashAddr..."
        & $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -w "$($BinFile.FullName)" $FlashAddr -v
        if ($LASTEXITCODE -eq 0) { Write-Host "✅ Flashed $DirName at $FlashAddr" -ForegroundColor Green } else { Write-Error "❌ Flash failed for $DirName" }
        return
    }

    # 2️⃣ Otherwise find a usable image (.bmp/.jpg/.png) even if _stm32.bmp
    $Image = Get-ChildItem -Path $Folder -File | Where-Object { $_.Extension -match '(?i)\.(bmp|jpg|jpeg|png)$' } | Select-Object -First 1
    if (-not $Image) {
        Write-Warning "No .bmp/.jpg/.png/.bin found in $DirName"
        return
    }

    $BaseName = [System.IO.Path]::GetFileNameWithoutExtension($Image.Name)
    $ResizedBMP = Join-Path $Folder ("{0}_stm32.bmp" -f $BaseName)
    $BinFilePath = Join-Path $Folder ("{0}.bin" -f $BaseName)

    Write-Host "Converting $($Image.Name) → $($ResizedBMP)..."
    try {
        magick "$($Image.FullName)" -resize 800x480! -depth 8 -type truecolor -compress none BMP3:"$ResizedBMP"
    } catch {
        Write-Error "❌ ImageMagick conversion failed for $DirName"
        return
    }

    if (-not (Test-Path $ResizedBMP)) {
        Write-Error "Conversion failed: $ResizedBMP not created"
        return
    }

    Copy-Item "$ResizedBMP" "$BinFilePath" -Force
    Write-Host "Flashing $($BinFilePath) to $FlashAddr..."
    & $STM32Prog -c port=SWD mode=HOTPLUG -el "$Loader" -w "$BinFilePath" $FlashAddr -v

    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ Flashed $DirName at $FlashAddr" -ForegroundColor Green
    } else {
        Write-Error "❌ Flash failed for $DirName"
    }
}

Write-Host "============================================="
Write-Host " STM32N6570-DK UI Image Flashing Utility"
Write-Host "============================================="

foreach ($entry in $FlashMap.GetEnumerator()) {
    Convert-And-Flash $entry.Key $entry.Value
}

Write-Host "`nAll flashing operations completed."
