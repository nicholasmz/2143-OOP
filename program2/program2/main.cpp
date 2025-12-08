/**
 * ============================================================
 *  SDL2 Shape Renderer
 * ============================================================
 *  This program loads Game of Life shapes from JSON and
 *  renders them visually using SDL2.
 *
 *  Concepts introduced:
 *   - Converting grid coordinates to pixel coordinates
 *   - Loading and parsing JSON shape data
 *   - Drawing filled rectangles for live cells
 *   - Centering shapes in the window
 *   - Handling keyboard input (ESC to quit)
 *   - Random color generation for shapes
 *
 *  Usage:
 *    program.exe [shape_name]
 *  Example:
 *    program.exe glider
 */

#include <SDL.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// ============================================================
// STRUCTURE DEFINITIONS
// ============================================================
/**
 * Represents a cell coordinate in the grid
 */
struct Cell {
    int x;
    int y;
};

/**
 * Represents a Game of Life shape
 */
struct Shape {
    int width;
    int height;
    vector<Cell> cells;
};

// ============================================================
// FUNCTION DECLARATIONS
// ============================================================

/**
 * Loads a shape from the JSON data by name
 * Returns a Shape struct, or throws if shape not found
 */
Shape loadShape(const json& data, const string& shapeName);

/**
 * Generates a random RGB color
 * Returns an SDL_Color with random R, G, B values
 */
SDL_Color getRandomColor();

/**
 * Calculates the offset to center a shape in a window
 */
void calculateCenterOffset(const Shape& shape, int cellSize, 
                           int windowWidth, int windowHeight,
                           int& offsetX, int& offsetY);

/**
 * Draws the shape on the renderer
 */
void drawShape(SDL_Renderer* renderer, const Shape& shape,
               int cellSize, int offsetX, int offsetY,
               SDL_Color color);

// ============================================================
// MAIN PROGRAM
// ============================================================

int main(int argc, char* argv[]) {
    // --------------------------------------------------------
    // CONFIGURATION SECTION
    // --------------------------------------------------------
    const int cellSize = 20;      // Each cell is 20x20 pixels
    const int windowWidth = 800;  // Window dimensions
    const int windowHeight = 600;

    // Default shape name (can be overridden by command line)
    string shapeName = "glider";

    // Check for command line argument
    if (argc > 1) {
        shapeName = argv[1];
    }

    // --------------------------------------------------------
    // LOAD JSON DATA
    // --------------------------------------------------------
    json data;
    try {
        ifstream f("shapes.json");
        if (!f.is_open()) {
            cerr << "Error: Cannot open shapes.json\n";
            return 1;
        }
        data = json::parse(f);
        f.close();
    } catch (const exception& e) {
        cerr << "Error parsing JSON: " << e.what() << "\n";
        return 1;
    }

    // --------------------------------------------------------
    // LOAD SHAPE
    // --------------------------------------------------------
    Shape shape;
    try {
        shape = loadShape(data, shapeName);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    cout << "Loaded shape: " << shapeName << "\n";
    cout << "Shape size: " << shape.width << "x" << shape.height << "\n";
    cout << "Live cells: " << shape.cells.size() << "\n";

    // --------------------------------------------------------
    // INITIALIZE SDL
    // --------------------------------------------------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "SDL Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    // Seed random number generator for color generation
    srand(static_cast<unsigned>(time(0)));

    // --------------------------------------------------------
    // CREATE THE WINDOW
    // --------------------------------------------------------
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Pattern Renderer",  // title
        SDL_WINDOWPOS_CENTERED,   // x position
        SDL_WINDOWPOS_CENTERED,   // y position
        windowWidth,              // window width (pixels)
        windowHeight,             // window height (pixels)
        SDL_WINDOW_SHOWN          // flags
    );

    if (!window) {
        cerr << "Window Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // --------------------------------------------------------
    // CREATE A RENDERER
    // --------------------------------------------------------
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        cerr << "Renderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // --------------------------------------------------------
    // GENERATE RANDOM COLOR FOR SHAPE
    // --------------------------------------------------------
    SDL_Color shapeColor = getRandomColor();

    // --------------------------------------------------------
    // CALCULATE CENTERING OFFSET
    // --------------------------------------------------------
    int offsetX = 0;
    int offsetY = 0;
    calculateCenterOffset(shape, cellSize, windowWidth, windowHeight,
                         offsetX, offsetY);

    // --------------------------------------------------------
    // MAIN LOOP
    // --------------------------------------------------------
    bool running = true;
    SDL_Event event;

    while (running) {
        // -------- EVENT HANDLING --------
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    // ESC key to quit
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    // R key to generate new random color
                    if (event.key.keysym.sym == SDLK_r) {
                        shapeColor = getRandomColor();
                    }
                    break;
            }
        }

        // -------- CLEAR SCREEN --------
        // Dark background
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        // -------- DRAW SHAPE --------
        drawShape(renderer, shape, cellSize, offsetX, offsetY, shapeColor);

        // -------- PRESENT RESULT --------
        SDL_RenderPresent(renderer);

        // -------- FRAME RATE LIMIT --------
        // Target 60 FPS (~16.6 ms per frame)
        SDL_Delay(16);
    }

    // --------------------------------------------------------
    // CLEANUP
    // --------------------------------------------------------
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// ============================================================
// FUNCTION IMPLEMENTATIONS
// ============================================================

/**
 * Loads a shape from the JSON data by name
 */
Shape loadShape(const json& data, const string& shapeName) {
    Shape result;

    // Verify the JSON has a shapes object
    if (!data.contains("shapes")) {
        throw runtime_error("JSON does not contain 'shapes' object");
    }

    auto& shapes = data["shapes"];

    // Check if the requested shape exists
    if (!shapes.contains(shapeName)) {
        throw runtime_error("Shape '" + shapeName + "' not found in shapes.json");
    }

    auto& shapeData = shapes[shapeName];

    // Extract shape properties from the size object
    auto& size = shapeData["size"];
    result.width = size["w"].get<int>();
    result.height = size["h"].get<int>();

    // Extract cells
    auto& cellsArray = shapeData["cells"];
    for (const auto& cellObj : cellsArray) {
        Cell cell;
        cell.x = cellObj["x"].get<int>();
        cell.y = cellObj["y"].get<int>();
        result.cells.push_back(cell);
    }

    return result;
}

/**
 * Generates a random RGB color
 */
SDL_Color getRandomColor() {
    SDL_Color color;
    // Generate bright colors (higher values)
    color.r = 100 + rand() % 156;  // 100-255
    color.g = 100 + rand() % 156;  // 100-255
    color.b = 100 + rand() % 156;  // 100-255
    color.a = 255;                 // Fully opaque
    return color;
}

/**
 * Calculates the offset to center a shape in a window
 */
void calculateCenterOffset(const Shape& shape, int cellSize,
                          int windowWidth, int windowHeight,
                          int& offsetX, int& offsetY) {
    // Calculate the total pixel size of the shape
    int shapePixelWidth = shape.width * cellSize;
    int shapePixelHeight = shape.height * cellSize;

    // Calculate the offset to center the shape
    offsetX = (windowWidth - shapePixelWidth) / 2;
    offsetY = (windowHeight - shapePixelHeight) / 2;
}

/**
 * Draws the shape on the renderer
 */
void drawShape(SDL_Renderer* renderer, const Shape& shape,
              int cellSize, int offsetX, int offsetY,
              SDL_Color color) {
    // Set the color for filled rectangles
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Draw each live cell as a filled rectangle
    for (const auto& cell : shape.cells) {
        // Convert grid coordinates to pixel coordinates
        int pixelX = offsetX + cell.x * cellSize;
        int pixelY = offsetY + cell.y * cellSize;

        // Create a rectangle for this cell
        SDL_Rect cellRect = {
            pixelX,
            pixelY,
            cellSize,
            cellSize
        };

        // Fill the rectangle
        SDL_RenderFillRect(renderer, &cellRect);
    }
}
