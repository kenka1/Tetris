#include <iostream>

#include "PlayerState/PlayerState.h"
#include "GameMode/GameMode.h"
#include "GameState/GameState.h"
#include "Actor/Actor.h"

int16_t PlayerState::Player_ID = -1;

PlayerState::PlayerState(GameMode* game):
    game(game), Grid_ID(4, -1), stop(false)
{
    std::cout << "Contructor PlayerState" << std::endl;
}

int16_t PlayerState::CalculateID(const glm::vec3& position)
{
    int16_t X = (position.x + 250) / 50;
    int16_t Y = (position.y + 500) / 50;

    return Y * 10 + X;
}