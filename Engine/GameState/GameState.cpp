#include <iostream>
#include <algorithm>

#include "GameState/GameState.h"
#include "Shape/Shape.h"
#include "ObjectsData/BlockData.h"

#include <glm/glm.hpp>

GameState::GameState():
    Grid(240), PredictGrid(4), size(0), Draw(5), Queue(0)
{
    std::cout << "Contructor GameState" << std::endl;
    Draw = {EForm::Square, EForm::Straight, EForm::T, EForm::L, EForm::Skew};
    for(size_t i = 0; i < 4; ++i)
    {
        Shape* target = new Shape();
        target->StoreData(sizeof(obj::data), obj::data);
        target->StoreIndices(sizeof(obj::indices), obj::indices);
        target->Scale(50.0f);
        target->UpdateTransform();
        target->LoadTexture("Textures/456.jpg");
        PredictGrid[i] = target;
    }
}

GameState::~GameState()
{
    for(size_t i = 0; i < 240; ++i)
    {
        if(Grid[i].Target != nullptr)
            delete Grid[i].Target;
    }

    for(size_t i = 0; i < 4; ++i)
        delete PredictGrid[i];
}

void GameState::AddToGrid(Shape* target, int16_t index, int16_t ID, EForm type_)
{
    Grid[index].Target = target;
    Grid[index].Player_ID = ID;
    Grid[index].type = type_;
}

void GameState::UpdatePredictGrid(const std::vector<glm::vec3>& offset)
{
    for(size_t i = 0; i < 4; ++i)
    {
        PredictGrid[i]->Translate(offset[i]);
        std::cout << "Offset : " << i << " " << "x : " << offset[i].x << " " << "y : " << offset[i].y << std::endl;
        PredictGrid[i]->UpdateTransform();
    }
}

bool GameState::CheckCell(int16_t index, int16_t id)
{
    if(index < 0)
        return true;
    if(Grid[index].Target != nullptr && Grid[index].Player_ID != id)
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
        std::cout << "REMOVE LINE" << std::endl;
        std::cout << "line index : " << line_index << std::endl;
        for(int i = line_index * 10; i < line_index * 10 + 10; ++i)
        {
            // std::cout << "Remove Index : " << i << std::endl;
            if(Grid[i].Target != nullptr)
                delete Grid[i].Target;
            ClearGrid(i);
        }
        score += 10;
        std::cout << "current scor : " << score << std::endl;
        ++remove_count;
        delay = true;
        if(_line_index == -1)
            _line_index = line_index;
        else
            _line_index = std::min(_line_index, line_index);
    }
}

void GameState::MoveLine()
{
    std::cout << "MOVE LINE" << std::endl;
    glm::vec3 offset(0.0f, -50.0f, 0.0f);
    offset.y *= remove_count;
    std::cout << "line index : " << _line_index << std::endl;
    std::cout << "offset : " << offset.y << std::endl;
    for(int i = _line_index * 10; i < 200 - 10 * remove_count; ++i)
    {
        Shape* target = Grid[i + 10 * remove_count].Target;
        Grid[i].Target = target;
        Grid[i].Player_ID = Grid[i + 10 * remove_count].Player_ID;
        Grid[i].stop = true;
        if(target != nullptr)
        {
            target->Translate(target->GetTranslate() + offset);
            target->UpdateTransform();
        }
    }
    delay = false;
    remove_count = 0;
    _line_index = -1;
}

void GameState::ResetDraw()
{
    std::swap<std::vector<EForm>>(Draw, Queue);
}