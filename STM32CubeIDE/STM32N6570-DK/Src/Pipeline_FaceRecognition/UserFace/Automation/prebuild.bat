@echo off
setlocal ENABLEDELAYEDEXPANSION
echo ============================================================
echo [PREBUILD] FaceRecognition Project Pre-Build
echo [PREBUILD] Date: %date%  Time: %time%
echo ============================================================

:: 1. Activate Python virtual environment
echo [PREBUILD] Activating Python virtual environment...
call "%~dp0.venv\Scripts\activate.bat"
if errorlevel 1 (
    echo [ERROR] Failed to activate .venv!
    exit /b 1
)

:: 2. Print TensorFlow version
python -c "import tensorflow as tf; print('[PREBUILD] TensorFlow version:', tf.__version__)" 2>nul
if errorlevel 1 (
    echo [ERROR] TensorFlow not found in .venv. Please run: pip install tensorflow
    exit /b 1
)

:: 3. Change to script directory and run Python
pushd "%~dp0"
echo [PREBUILD] Running gen_refset_tflite.py from: %CD%
python gen_refset_tflite.py
if errorlevel 1 (
    echo [ERROR] Python preprocessing failed!
    popd
    exit /b 1
)
popd

:: 3.5 Copy generated embeddings to STM32 project source folder
set SRC_DIR=%~dp0output
set DST_DIR=%~dp0..\..\Generated

if exist "%SRC_DIR%\embeddings_table.c" (
    echo [PREBUILD] Copying embeddings_table.c to project Generated folder...
    if not exist "%DST_DIR%" mkdir "%DST_DIR%"
    copy /Y "%SRC_DIR%\embeddings_table.c" "%DST_DIR%\embeddings_table.c" >nul
    echo [PREBUILD] embeddings_table.c copied successfully.
) else (
    echo [WARNING] embeddings_table.c not found in output folder!
)

:: (Optional) Copy embeddings.txt for reference
if exist "%SRC_DIR%\embeddings.txt" (
    copy /Y "%SRC_DIR%\embeddings.txt" "%DST_DIR%\embeddings.txt" >nul
    echo [PREBUILD] embeddings.txt copied successfully.
)

:: 4. Log success and force clean exit
for /f "tokens=1-4 delims=:.," %%a in ("%time%") do set HOUR=%%a& set MIN=%%b& set SEC=%%c
set NOW=%date% %HOUR%:%MIN%:%SEC%
echo [PREBUILD] ✅ Completed successfully at %NOW%
echo ============================================================

endlocal
exit /b 0
