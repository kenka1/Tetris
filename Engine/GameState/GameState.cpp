#include <iostream>
#include <algorithm>

#include "GameState/GameState.h"
#include "Shape/Shape.h"

#include <glm/glm.hpp>

GameState::GameState():
    Grid(210), size(0), Draw(5), Queue(0)
{
    std::cout << "Contructor GameState" << std::endl;
    Draw = {EForm::Square, EForm::Straight, EForm::T, EForm::L, EForm::Skew};
}

GameState::~GameState()
{
    for(int i = 0; i < 200; ++i)
    {
        if(Grid[i].Target != nullptr)
            delete Grid[i].Target;
    }
}

void GameState::AddToGrid(Shape* target, int16_t index, int16_t ID)
{
    Grid[index].Target = target;
    Grid[index].Player_ID = ID;
}

bool GameState::CheckCell(int16_t index, int16_t ID)
{
    if(index < 0)
        return true;
    if(Grid[index].Target != nullptr && Grid[index].Player_ID != ID)
        return true;
    return false;
}

void GameState::RemoveLine(int16_t index)
{
    int16_t line_index = (index - index % 10) / 10;
    bool remove = true;
    for(int i = line_index * 10; i < line_index * 10 + 10; ++i)
        if(Grid[i].Target == nullptr)
            remove = false;
    // std::cout << "LINE INDEX : " << line_index << std::endl;
    if(remove)
    {
        // std::cout << "REMOVE LINE" << std::endl;
        for(int i = line_index * 10; i < line_index * 10 + 10; ++i)
        {
            // std::cout << "Remove Index : " << i << std::endl;
            if(Grid[i].Target != nullptr)
                delete Grid[i].Target;
            ClearGrid(i);
        }
    }
}

void GameState::MoveLine(int16_t line_index)
{
    for(int i = line_index * 10; i < 200 - 10; ++i)
    {
        Shape* target = Grid[i + 10].Target;
        if(target != nullptr)
        {
            Grid[i] = Grid[i + 10];
        }
    }
}

void GameState::ResetDraw()
{
    std::swap<std::vector<EForm>>(Draw, Queue);
}