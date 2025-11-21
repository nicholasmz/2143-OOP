@echo off
REM Download the complete nlohmann/json library first
REM This script compiles with MSVC

cl /EHsc /std:c++17 /I includes main.cpp /FeProgram1_full.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    Program1_full.exe
) else (
    echo Build failed!
)
