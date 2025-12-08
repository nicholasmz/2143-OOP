#pragma once

#include "CellularAutomaton.hpp"

class ConwayLife : public CellularAutomaton {
   public:
    ConwayLife(int r, int c);
    void step() override;           // Conway's rules
    void display() const override;  // ASCII visualization
};
