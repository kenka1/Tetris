#include <iostream>

#include "PlayerController/PlayerController.h"
#include "Shape/Shape.h"

PlayerController::PlayerController(GameMode* game):
    game(game), player(nullptr)
{
    std::cout << "Contructor PlayerController" << std::endl;
}

glm::vec3 PlayerController::Move(int8_t call) const
{
    float step = 50.0f;
    glm::vec3 pos(0.0);
    switch(call)
    {
    case 0:
        pos.x += step;
        break;
    case 1:
        pos.x -= step;
        break;
    case 2:
        pos.y -= step;
        break;
    case 3:
        // pos.y += step;
        break;
    case 4:
        pos.x += step;
        pos.y -= step;
        break;
    case 5:
        pos.x -= step;
        pos.y -= step;
        break;
    }
    return pos;
}