#pragma once

#include <vector>
#include <cstdint>

class Shape;

class GameState
{
public:
    GameState();
    ~GameState();

    void AddToGrid(Shape*, uint8_t);
    bool CheckCell(uint8_t);
private:
    std::vector<Shape*> Grid;
};