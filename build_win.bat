@echo off
:: Build script for Saleae Logic Analyzer Plugin on Windows
:: This script uses the tools bundled with Visual Studio 2022 Community

set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
set "VCVARS=%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
set "CMAKE_PATH=%VS_PATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"

echo ===================================================
echo Saleae Plugin Build Script
echo ===================================================

if not exist "%VCVARS%" (
    echo [ERROR] Could not find Visual Studio 2022 Community at:
    echo %VS_PATH%
    echo Please check your Visual Studio installation path.
    pause
    exit /b 1
)

echo [1/4] Setting up MSVC environment (x64)...
call "%VCVARS%" x64

echo [2/4] Adding bundled CMake to PATH...
set "PATH=%CMAKE_PATH%;%PATH%"

if not exist build (
    echo [3/4] Creating build directory...
    mkdir build
)
cd build

echo [4/4] Running CMake and Building...
cmake .. -A x64
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo ===================================================
echo SUCCESS! 
echo Your DLL should be in: build\Analyzers\Release\SimpleParallelNoClock.dll
echo ===================================================
cd ..
pause
