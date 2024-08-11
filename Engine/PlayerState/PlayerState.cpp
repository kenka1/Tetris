#include "PlayerState/PlayerState.h"
#include "GameMode/GameMode.h"
#include "GameState/GameState.h"
#include "Actor/Actor.h"

#include <glm/glm.hpp>

PlayerState::PlayerState(GameMode* game):
    id(-1), stop(false), game(game)
{}

int16_t PlayerState::CalculateID(Actor* target)
{
    glm::vec3 position = target->GetTranslate();
    int16_t X = (position.x + 250) / 50;
    int16_t Y = (position.y + 500) / 50;
    int16_t new_id = Y * 10 + X;

    GameState* _GameState = game->GetGameState();
    if(!_GameState)
        return id;

    if(new_id >= 200 || new_id < 0 || (new_id >= 10 && _GameState->CheckCell(new_id)))
    {
        stop = true;
        game->GetGameState()->AddToGrid(target, id);
        return id;
    }

    return Y * 10 + X;
}