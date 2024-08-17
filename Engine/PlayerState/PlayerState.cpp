#include "PlayerState/PlayerState.h"
#include "GameMode/GameMode.h"
#include "GameState/GameState.h"
#include "Actor/Actor.h"

PlayerState::PlayerState(GameMode* game):
    game(game), id(4, -1), stop(false)
{}

void PlayerState::SetID(int16_t new_id, size_t index)
{
    id[index] = new_id;
}

int16_t PlayerState::CalculateID(const glm::vec3& position)
{
    int16_t X = (position.x + 250) / 50;
    int16_t Y = (position.y + 500) / 50;

    return Y * 10 + X;
}