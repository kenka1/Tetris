#include "PlayerController/PlayerController.h"
#include "Shape/Shape.h"
#include "GameMode/GameMode.h"

PlayerController::PlayerController(Shape* player, GameMode* game)
    :player(player), game(game)
{}

void PlayerController::Check()
{

}