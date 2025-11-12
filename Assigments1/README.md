# Program_01 — JSON Loader Assignment

This project loads and displays Game of Life shapes stored in a JSON file.

## Files
- `main.cpp` — main program
- `shapes.json` — shape data (includes "dot" shape)
- `Makefile` — build system
- `includes/json.hpp` — download required JSON library

## Build & Run
Linux/macOS/WSL:
```bash
chmod +x fetch_json_header.sh
./fetch_json_header.sh
make
make run
```

Windows PowerShell:
```powershell
.\fetch_json_header.ps1
make
.\Program_01_JSON_Loader.exe
```

## Output Example
```
Available shapes:
 - block
 - beehive
 - glider
 - dot

Total shapes loaded: 4

Enter shape name: glider
glider (3x3)
#..
.#.
###
```
