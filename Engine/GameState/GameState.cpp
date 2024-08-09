#include "GameState/GameState.h"
#include "Shape/Shape.h"

GameState::GameState():
    Grid(200)
{}

GameState::~GameState()
{
    Grid.clear();
}

void GameState::AddToGrid(Shape* target, uint8_t index)
{
    Grid[index] = target;
}

bool GameState::CheckCell(uint8_t index)
{
    if(Grid[index] == nullptr)
        return false;
    return true;
}