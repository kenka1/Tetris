#pragma once

#include <glm/glm.hpp>

class Actor;
class GameMode;

class PlayerController
{
public:
    PlayerController(GameMode*);
    ~PlayerController() = default;

    inline void SetPlayer(Actor* target) {player = target;}
    inline Actor* GetPlayer() const {return player;}
private:
    GameMode* game;
    Actor* player;
};