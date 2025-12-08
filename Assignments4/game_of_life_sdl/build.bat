@echo off
REM Build script for Conway's Game of Life SDL2

echo ========================================
echo Building Conway's Game of Life - SDL2
echo ========================================
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Run CMake
echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022"

if errorlevel 1 (
    echo.
    echo CMake configuration failed!
    echo Make sure SDL2 is installed (vcpkg install sdl2)
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

if errorlevel 1 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo ========================================
echo.
echo Executable: build\Release\SDL_GOL_main.exe
echo.
echo To run:
echo   cd build\Release
echo   .\SDL_GOL_main.exe
echo.
echo Or with custom parameters:
echo   .\SDL_GOL_main.exe window_width=1024 window_height=768 cellSize=10
echo.
echo Controls:
echo   SPACE - Pause/Resume
echo   N     - Step one generation
echo   R     - Randomize
echo   C     - Clear
echo   G     - Load glider at mouse
echo   B     - Load blinker at mouse
echo   T     - Load toad at mouse
echo   L     - Toggle grid lines
echo   Q/ESC - Quit
echo   Click - Toggle cell
echo.
pause
