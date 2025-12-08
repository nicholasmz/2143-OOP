@echo off
REM Build script for SDL2 Pattern Renderer on Windows

echo Building Pattern Renderer...
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Run CMake
cmake .. -G "Visual Studio 17 2022"

if errorlevel 1 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM Build the project
cmake --build . --config Release

if errorlevel 1 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Build successful! The executable is in build\Release\program2.exe
echo To run the program: .\Release\program2.exe [pattern_name]
echo.
echo Available patterns:
echo   - glider (default)
echo   - blinker
echo   - toad
echo   - beacon
echo   - block
echo   - pulsar
echo.
pause
