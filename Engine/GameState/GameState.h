#pragma once

#include <vector>
#include <cstdint>

class Actor;

class GameState
{
public:
    GameState();
    ~GameState();

    void AddToGrid(Actor*, uint8_t);
    bool CheckCell(uint8_t);
private:
    std::vector<Actor*> Grid;
    size_t size;
};