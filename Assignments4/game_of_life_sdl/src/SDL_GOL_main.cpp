/**
 * ============================================================
 *  SDL2 Conway's Game of Life - Full Interactive Version
 * ============================================================
 *  
 *  Features:
 *   - SDL2 graphical display
 *   - Mouse clicking to toggle cells
 *   - Keyboard controls (pause, step, randomize, clear, quit)
 *   - JSON pattern loader
 *   - Command-line configuration
 *   - Color customization
 * 
 *  Controls:
 *   - SPACE: Pause/Resume simulation
 *   - N: Step one generation (when paused)
 *   - R: Randomize grid
 *   - C: Clear grid
 *   - G: Load glider pattern at mouse position
 *   - B: Load blinker pattern at mouse position
 *   - T: Load toad pattern at mouse position
 *   - L: Toggle grid lines
 *   - Q/ESC: Quit
 *   - Left Mouse: Toggle cell
 * 
 *  Usage:
 *    ./SDL_GOL_main window_width=1024 window_height=768 cellSize=10 frameDelayMs=50
 * 
 */

#include <SDL.h>
#include <fstream>
#include <iostream>

#include "../includes/ArgsToJson.hpp"
#include "../includes/AutomatonUtils.hpp"
#include "../includes/ConwayLife.hpp"
#include "../includes/SdlScreen.hpp"
#include "../includes/json.hpp"

using namespace std;
using nlohmann::json;

// --------------------------------------------------------------
// Default simulation parameters
// --------------------------------------------------------------
json defaults = {
    {"window_width", 900},
    {"window_height", 900},
    {"cellSize", 12},
    {"frameDelayMs", 50}
};

// --------------------------------------------------------------
// Pattern Loader: Load patterns from JSON file
// --------------------------------------------------------------
json loadPatternsFromFile(const string& filename) {
    try {
        ifstream f(filename);
        if (!f.is_open()) {
            cerr << "Warning: Cannot open " << filename << endl;
            return json::object();
        }
        json data = json::parse(f);
        f.close();
        return data;
    } catch (const exception& e) {
        cerr << "Error loading patterns: " << e.what() << endl;
        return json::object();
    }
}

// --------------------------------------------------------------
// Place Pattern: Insert a pattern into the grid at mouse position
// --------------------------------------------------------------
void placePattern(ConwayLife& gol, const json& patternData, int mouseGridX, int mouseGridY) {
    try {
        // Extract pattern info
        auto& size = patternData["size"];
        int w = size["w"].get<int>();
        int h = size["h"].get<int>();
        
        auto& cells = patternData["cells"];
        
        // Get grid reference
        auto& grid = gol.getGridMutable();
        int gridRows = gol.getRows();
        int gridCols = gol.getCols();
        
        // Place each cell relative to mouse position
        for (const auto& cell : cells) {
            int x = cell["x"].get<int>();
            int y = cell["y"].get<int>();
            
            // Adjust to mouse position
            int gridX = mouseGridX + x;
            int gridY = mouseGridY + y;
            
            // Bounds check
            if (gridY >= 0 && gridY < gridRows && gridX >= 0 && gridX < gridCols) {
                grid[gridY][gridX] = 1;
            }
        }
        
        cout << "Pattern placed at (" << mouseGridX << ", " << mouseGridY << ")" << endl;
    } catch (const exception& e) {
        cerr << "Error placing pattern: " << e.what() << endl;
    }
}

// --------------------------------------------------------------
// Main Program
// --------------------------------------------------------------
int main(int argc, char* argv[]) {
    // Parse command-line arguments
    json params = ArgsToJson(argc, argv);
    
    // Merge with defaults
    for (auto& [key, value] : defaults.items()) {
        if (params.find(key) == params.end()) {
            params[key] = value;
        }
    }
    
    cout << "=== Conway's Game of Life - SDL2 Edition ===" << endl;
    cout << "Simulation Parameters:" << endl;
    cout << params.dump(2) << endl;
    cout << endl;
    
    // Extract parameters
    int windowWidth = params["window_width"].get<int>();
    int windowHeight = params["window_height"].get<int>();
    int cellSize = params["cellSize"].get<int>();
    int frameDelayMs = params["frameDelayMs"].get<int>();
    
    // Create SDL screen
    SdlScreen screen(windowWidth, windowHeight, cellSize);
    
    // Calculate grid dimensions
    int gridRows = screen.getGridRows();
    int gridCols = screen.getGridCols();
    
    cout << "Grid size: " << gridRows << " rows x " << gridCols << " cols" << endl;
    
    // Create Conway's Life automaton
    ConwayLife gol(gridRows, gridCols);
    
    // Load patterns from JSON
    json patterns = loadPatternsFromFile("../assets/shapes.json");
    if (!patterns.contains("shapes")) {
        patterns = loadPatternsFromFile("assets/shapes.json");
    }
    if (!patterns.contains("shapes")) {
        patterns = loadPatternsFromFile("shapes.json");
    }
    
    bool patternsLoaded = patterns.contains("shapes");
    if (patternsLoaded) {
        cout << "Patterns loaded successfully!" << endl;
    } else {
        cout << "Warning: Could not load shapes.json" << endl;
    }
    
    // Print controls
    cout << "\n=== CONTROLS ===" << endl;
    cout << "SPACE: Pause/Resume" << endl;
    cout << "N: Step one generation (when paused)" << endl;
    cout << "R: Randomize grid" << endl;
    cout << "C: Clear grid" << endl;
    cout << "G: Load glider at mouse" << endl;
    cout << "B: Load blinker at mouse" << endl;
    cout << "T: Load toad at mouse" << endl;
    cout << "L: Toggle grid lines" << endl;
    cout << "Q/ESC: Quit" << endl;
    cout << "Left Click: Toggle cell" << endl;
    cout << "==================\n" << endl;
    
    // Simulation state
    bool running = true;
    bool paused = false;
    int generation = 0;
    
    SDL_Event event;
    
    // Main loop
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            paused = !paused;
                            cout << (paused ? "PAUSED" : "RUNNING") << endl;
                            break;
                            
                        case SDLK_n:
                            if (paused) {
                                gol.step();
                                generation++;
                                cout << "Generation: " << generation << endl;
                            }
                            break;
                            
                        case SDLK_r:
                            gol.randomize(0.25);
                            generation = 0;
                            cout << "Grid randomized" << endl;
                            break;
                            
                        case SDLK_c:
                            gol.clear();
                            generation = 0;
                            cout << "Grid cleared" << endl;
                            break;
                            
                        case SDLK_l:
                            {
                                static bool gridLines = true;
                                gridLines = !gridLines;
                                screen.setShowGridLines(gridLines);
                                cout << "Grid lines: " << (gridLines ? "ON" : "OFF") << endl;
                            }
                            break;
                            
                        case SDLK_g:
                            if (patternsLoaded && patterns["shapes"].contains("glider")) {
                                int mouseX, mouseY;
                                SDL_GetMouseState(&mouseX, &mouseY);
                                int gridX = mouseX / cellSize;
                                int gridY = mouseY / cellSize;
                                placePattern(gol, patterns["shapes"]["glider"], gridX, gridY);
                            }
                            break;
                            
                        case SDLK_b:
                            if (patternsLoaded && patterns["shapes"].contains("blinker")) {
                                int mouseX, mouseY;
                                SDL_GetMouseState(&mouseX, &mouseY);
                                int gridX = mouseX / cellSize;
                                int gridY = mouseY / cellSize;
                                placePattern(gol, patterns["shapes"]["blinker"], gridX, gridY);
                            }
                            break;
                            
                        case SDLK_t:
                            if (patternsLoaded && patterns["shapes"].contains("toad")) {
                                int mouseX, mouseY;
                                SDL_GetMouseState(&mouseX, &mouseY);
                                int gridX = mouseX / cellSize;
                                int gridY = mouseY / cellSize;
                                placePattern(gol, patterns["shapes"]["toad"], gridX, gridY);
                            }
                            break;
                            
                        case SDLK_q:
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                    }
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // Toggle cell at mouse position
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        
                        int gridX = mouseX / cellSize;
                        int gridY = mouseY / cellSize;
                        
                        if (gridY >= 0 && gridY < gridRows && gridX >= 0 && gridX < gridCols) {
                            auto& grid = gol.getGridMutable();
                            grid[gridY][gridX] = 1 - grid[gridY][gridX];  // Toggle
                        }
                    }
                    break;
            }
        }
        
        // Update simulation (if not paused)
        if (!paused) {
            gol.step();
            generation++;
        }
        
        // Render
        screen.render(gol.getGrid());
        
        // Frame rate limiting
        screen.pause(frameDelayMs);
    }
    
    cout << "\nSimulation ended at generation " << generation << endl;
    cout << "Goodbye!" << endl;
    
    return 0;
}
