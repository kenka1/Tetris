#pragma once

class Actor;
class GameMode;

class PlayerController
{
public:
    PlayerController(GameMode*);
    ~PlayerController() = default;

    inline void SetPlayer(Actor* target) {player = target;}
    inline Actor* GetPlayer() const {return player;}
    void Check();
private:
    GameMode* game;
    Actor* player;
};