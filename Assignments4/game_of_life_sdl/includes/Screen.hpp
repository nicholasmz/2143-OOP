#pragma once
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

// --------------------------------------------------------------
// Base Class: Screen
// --------------------------------------------------------------
// Purpose:
//   This class defines a generic rendering interface for anything
//   that displays a 2D cellular automaton (or any grid of ints).
//
// Why this is useful:
//   - Allows multiple display backends (text, SDL2, ncurses, GUI)
//   - Code that runs the automaton does NOT depend on how it is drawn
//   - Encourages clean separation of "model" vs "view"
//
// The grid is represented as a vector of vector<int>, where
// each element is the state of a cell (0, 1, or other integers).
// --------------------------------------------------------------
class Screen {
   public:
    // ----------------------------------------------------------
    // render():
    //   Pure virtual function that must be implemented by any
    //   subclass. Responsible for drawing the given 2D grid.
    //
    //   The 'const' means the method does not modify the Screen.
    // ----------------------------------------------------------
    virtual void render(const std::vector<std::vector<int>>& grid) const = 0;

    // ----------------------------------------------------------
    // pause():
    //   Pure virtual. Allows a screen to control timing, frame rate,
    //   or animation pacing by sleeping for 'ms' milliseconds.
    // ----------------------------------------------------------
    virtual void pause(int ms) const = 0;

    // ----------------------------------------------------------
    // Virtual destructor:
    //   Ensures safe cleanup when deleting derived classes through
    //   a base pointer — required for polymorphic classes.
    // ----------------------------------------------------------
    virtual ~Screen() = default;
};

// --------------------------------------------------------------
// TextScreen:
// --------------------------------------------------------------
// A concrete implementation of Screen that renders a grid directly
// to the terminal using ASCII or Unicode characters.
//
// Useful for:
//   - quick demos
//   - debugging visual behavior
//   - environments where no graphics library is available
// --------------------------------------------------------------
class TextScreen : public Screen {
   public:
    void render(const std::vector<std::vector<int>>& grid) const override;
    void pause(int ms) const override;
};
