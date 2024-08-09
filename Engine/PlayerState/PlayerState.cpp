#include "PlayerState/PlayerState.h"

PlayerState::PlayerState():
    id(-1)
{}

int16_t PlayerState::CalculateID(const glm::vec3& position)
{
    int16_t X = (position.x + 250) / 50;
    int16_t Y = (position.y + 500) / 50;
    return Y * 10 + X;
}