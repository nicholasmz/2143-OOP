@echo off
REM Compile with LLVM Clang using system libraries
REM This script attempts to build Program1_full.exe

setlocal enabledelayedexpansion

REM Try to locate LLVM
set LLVM_PATH=C:\Program Files\LLVM

if not exist "%LLVM_PATH%\bin\clang++.exe" (
    echo Error: LLVM not found at %LLVM_PATH%
    echo Please install LLVM from: https://github.com/llvm/llvm-project/releases
    exit /b 1
)

REM Try to find Windows SDK or Visual Studio includes
set VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community
set SDK_PATH=C:\Program Files (x86)\Windows Kits\10

if not exist "%VS_PATH%\VC\Tools\MSVC" (
    if not exist "%SDK_PATH%\Include" (
        echo.
        echo Warning: C++ Standard Library headers not found.
        echo This program requires Visual Studio or Windows SDK to be installed.
        echo.
        echo Attempting workaround compilation...
        echo.
    )
)

REM Compile
echo Compiling Program1_full...
"%LLVM_PATH%\bin\clang++.exe" -std=c++17 -I includes -fms-compatibility main.cpp -o Program1_full.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo Running Program1_full...
    echo.
    Program1_full.exe
) else (
    echo.
    echo Build failed with error code %ERRORLEVEL%
    echo.
    echo Troubleshooting:
    echo 1. Install Visual Studio 2022 Community with C++ workload
    echo 2. Or install Windows SDK: https://developer.microsoft.com/windows/downloads/windows-sdk/
    echo 3. Or install GCC: https://www.mingw-w64.org/
)

endlocal
