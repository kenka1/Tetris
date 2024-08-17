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

void GameState::AddToGrid(Shape* target, int16_t index)
{
    Grid[index] = target;
}

bool GameState::CheckCell(int16_t index)
{
    if(index >= 200 || index < 0)
        return true;
    if(Grid[index] != nullptr)
        return true;
    return false;
}

void GameState::RemoveLine(int16_t index)
{
    int16_t line_index = (index - index % 10) / 10;
    bool remove = true;
    for(int i = line_index * 10; i < line_index * 10 + 10; ++i)
        if(Grid[i] == nullptr)
            remove = false;
    std::cout << "LINE INDEX : " << line_index << std::endl;
    if(remove)
    {
        std::cout << "REMOVE LINE" << std::endl;
        for(int i = line_index * 10; i < line_index * 10 + 10; ++i)
        {
            delete Grid[i];
            Grid[i] = nullptr;
        }
    }
}