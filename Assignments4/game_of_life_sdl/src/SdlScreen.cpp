#include "../includes/SdlScreen.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// --------------------------------------------------------------
// Constructor: Initialize SDL2 window and renderer
// --------------------------------------------------------------
SdlScreen::SdlScreen(int width, int height, int cellSz)
    : cellSize(cellSz), windowWidth(width), windowHeight(height), showGridLines(true) {
    
    // Calculate grid dimensions
    gridCols = width / cellSize;
    gridRows = height / cellSize;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize SDL");
    }
    
    // Create window
    window = SDL_CreateWindow(
        "Conway's Game of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw std::runtime_error("Failed to create window");
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Failed to create renderer");
    }
    
    // Set default colors
    aliveColor = {255, 255, 255, 255};      // White
    deadColor = {30, 30, 40, 255};          // Dark blue-gray
    gridLineColor = {80, 80, 100, 255};     // Light gray
    backgroundColor = {30, 30, 40, 255};    // Same as dead color
}

// --------------------------------------------------------------
// Destructor: Clean up SDL resources
// --------------------------------------------------------------
SdlScreen::~SdlScreen() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

// --------------------------------------------------------------
// render(): Draw the grid
// --------------------------------------------------------------
void SdlScreen::render(const std::vector<std::vector<int>>& grid) const {
    // Clear screen with background color
    SDL_SetRenderDrawColor(renderer, 
                          backgroundColor.r, 
                          backgroundColor.g, 
                          backgroundColor.b, 
                          backgroundColor.a);
    SDL_RenderClear(renderer);
    
    // Draw cells
    int rows = grid.size();
    int cols = rows > 0 ? grid[0].size() : 0;
    
    for (int r = 0; r < rows && r < gridRows; ++r) {
        for (int c = 0; c < cols && c < gridCols; ++c) {
            SDL_Rect cellRect = {
                c * cellSize,
                r * cellSize,
                cellSize,
                cellSize
            };
            
            if (grid[r][c] == 1) {
                // Draw alive cell
                SDL_SetRenderDrawColor(renderer,
                                      aliveColor.r,
                                      aliveColor.g,
                                      aliveColor.b,
                                      aliveColor.a);
                SDL_RenderFillRect(renderer, &cellRect);
            } else {
                // Draw dead cell (optional, background already set)
                SDL_SetRenderDrawColor(renderer,
                                      deadColor.r,
                                      deadColor.g,
                                      deadColor.b,
                                      deadColor.a);
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }
    
    // Draw grid lines if enabled
    if (showGridLines) {
        SDL_SetRenderDrawColor(renderer,
                              gridLineColor.r,
                              gridLineColor.g,
                              gridLineColor.b,
                              gridLineColor.a);
        
        // Vertical lines
        for (int x = 0; x <= windowWidth; x += cellSize) {
            SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
        }
        
        // Horizontal lines
        for (int y = 0; y <= windowHeight; y += cellSize) {
            SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
        }
    }
    
    // Present to screen
    SDL_RenderPresent(renderer);
}

// --------------------------------------------------------------
// pause(): Frame rate limiting
// --------------------------------------------------------------
void SdlScreen::pause(int ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// --------------------------------------------------------------
// isOpen(): Check if window is still valid
// --------------------------------------------------------------
bool SdlScreen::isOpen() const {
    return window != nullptr && renderer != nullptr;
}

// --------------------------------------------------------------
// setAliveColor(): Change color of alive cells
// --------------------------------------------------------------
void SdlScreen::setAliveColor(Uint8 r, Uint8 g, Uint8 b) {
    aliveColor = {r, g, b, 255};
}

// --------------------------------------------------------------
// setDeadColor(): Change color of dead cells
// --------------------------------------------------------------
void SdlScreen::setDeadColor(Uint8 r, Uint8 g, Uint8 b) {
    deadColor = {r, g, b, 255};
    backgroundColor = deadColor;
}

// --------------------------------------------------------------
// setShowGridLines(): Toggle grid lines
// --------------------------------------------------------------
void SdlScreen::setShowGridLines(bool show) {
    showGridLines = show;
}
