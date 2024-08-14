#include <iostream>

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

void GameState::AddToGrid(Actor* target, int16_t index)
{
    Grid[index] = target;
}

bool GameState::CheckCell(int16_t index)
{
    //debug
    for(int i = 0; i < 200; ++i)
        if(Grid[i] != nullptr)
            std::cout << "busy : " << i << std::endl;
    //debug end
    if(index >= 200 || index < 0)
        return true;
    if(Grid[index] != nullptr)
        return true;
    return false;
}