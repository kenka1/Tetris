#include "GameState/GameState.h"
#include "Shape/Shape.h"

GameState::GameState():
    Grid(200), size(0)
{}

GameState::~GameState()
{
    for(int i = 0; i < 200; ++i)
    {
        if(Grid[i] != nullptr)
            delete Grid[i];
    }
}

void GameState::AddToGrid(Actor* target, uint8_t index)
{
    Grid[index] = target;
}

bool GameState::CheckCell(uint8_t index)
{
    if(Grid[index] == nullptr)
        return false;
    return true;
}