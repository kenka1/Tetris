#include <iostream>

#include "PlayerController/PlayerController.h"
#include "Shape/Shape.h"
#include "GameMode/GameMode.h"

PlayerController::PlayerController(GameMode* game):
    game(game), player(nullptr)
{
    std::cout << "Contructor PlayerController" << std::endl;
}