#pragma once
#include "Type/Types.h"

#include <glm/glm.hpp>

class BaseActor;
class GameMode;

class PlayerController
{
public:
    PlayerController(GameMode*);
    ~PlayerController() = default;

    inline void SetPlayer(BaseActor* target) {player = target;}
    inline BaseActor* GetPlayer() {return player;}
    glm::vec3 Move(int8_t) const;
private:
    GameMode* game;
    BaseActor* player;
};