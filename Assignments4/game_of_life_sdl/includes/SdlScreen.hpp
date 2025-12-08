#pragma once

#include "Screen.hpp"
#include <SDL.h>
#include <vector>

// --------------------------------------------------------------
// SdlScreen:
// --------------------------------------------------------------
// SDL2-based implementation of the Screen interface.
// Renders the cellular automaton grid as filled rectangles.
//
// Features:
//   - Grid visualization with configurable cell size
//   - Optional grid lines
//   - Color customization for alive/dead cells
//   - Frame rate limiting
// --------------------------------------------------------------
class SdlScreen : public Screen {
   private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int cellSize;
    int windowWidth;
    int windowHeight;
    int gridRows;
    int gridCols;
    
    // Colors
    SDL_Color aliveColor;
    SDL_Color deadColor;
    SDL_Color gridLineColor;
    SDL_Color backgroundColor;
    
    bool showGridLines;

   public:
    // Constructor: initializes SDL window and renderer
    SdlScreen(int width, int height, int cellSz);
    
    // Destructor: cleans up SDL resources
    ~SdlScreen();
    
    // Implement Screen interface
    void render(const std::vector<std::vector<int>>& grid) const override;
    void pause(int ms) const override;
    
    // Additional SDL-specific methods
    bool isOpen() const;
    void setAliveColor(Uint8 r, Uint8 g, Uint8 b);
    void setDeadColor(Uint8 r, Uint8 g, Uint8 b);
    void setShowGridLines(bool show);
    
    // Get grid dimensions
    int getGridRows() const { return gridRows; }
    int getGridCols() const { return gridCols; }
    int getCellSize() const { return cellSize; }
};
