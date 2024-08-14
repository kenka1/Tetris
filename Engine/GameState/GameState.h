#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

class Actor;

class GameState
{
public:
    GameState();
    ~GameState();

    void AddToGrid(Actor*, int16_t);
    bool CheckCell(int16_t);
    inline std::vector<Actor*>& GetGrid() {return Grid;}
private:
    std::vector<Actor*> Grid;
    size_t size;
};