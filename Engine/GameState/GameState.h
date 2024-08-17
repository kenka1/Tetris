#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

class Shape;

class GameState
{
public:
    GameState();
    ~GameState();

    void AddToGrid(Shape*, int16_t);
    bool CheckCell(int16_t);
    void RemoveLine(int16_t);
    inline std::vector<Shape*>& GetGrid() {return Grid;}
private:
    std::vector<Shape*> Grid;
    size_t size;
};