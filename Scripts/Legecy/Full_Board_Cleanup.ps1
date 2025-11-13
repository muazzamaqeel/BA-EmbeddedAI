# ================================================================
# STM32N6570-DK — Full NOR Flash Memory Cleanup (Encoding-Safe)
# ================================================================

$ErrorActionPreference = 'Stop'
$STM32Prog = 'STM32_Programmer_CLI'

$configPath = Join-Path $PSScriptRoot "config.json"
if (-Not (Test-Path $configPath)) {
    Write-Error "Config file not found: $configPath"
    exit
}

try {
    $config = Get-Content $configPath -Raw | ConvertFrom-Json
    $Loader = $config.LoaderPath
}
catch {
    Write-Error "Failed to read config.json. Ensure it's valid JSON."
    exit
}

Clear-Host
Write-Host ''
Write-Host ' STM32N6570-DK — FULL NOR MEMORY CLEANUP'
Write-Host ''

try {
    $version = & $STM32Prog --version
    Write-Host "STM32CubeProgrammer detected: $version"
}
catch {
    Write-Error 'STM32_Programmer_CLI not found. Install STM32CubeProgrammer and retry.'
    Write-Host ''
    [void][System.Console]::WriteLine()
    Write-Host 'Press ENTER to close this window'
    [void][System.Console]::ReadLine()
    exit
}

Write-Host ''
Write-Host 'Starting full external NOR erase...'
Write-Host 'This may take several minutes. Do not disconnect power or ST-LINK.'
Write-Host ''

& $STM32Prog -c port=SWD mode=HOTPLUG -el $Loader -e all

if ($LASTEXITCODE -ne 0) {
    Write-Error 'NOR flash erase failed.'
    Write-Host ''
    Write-Host 'Press ENTER to close this window'
    [void][System.Console]::ReadLine()
    exit
}

Write-Host ''
Write-Host 'Verifying NOR memory (first 16 words at 0x70000000)...'
& $STM32Prog -c port=SWD mode=UR -el $Loader -r32 0x70000000 16

Write-Host ''
Write-Host '=============================================================='
Write-Host '   COMPLETE NOR MEMORY ERASE FINISHED SUCCESSFULLY'
Write-Host '=============================================================='
Write-Host ''
Write-Host 'Press ENTER to close this window'
[void][System.Console]::ReadLine()
