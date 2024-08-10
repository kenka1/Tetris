#pragma once

class Shape;
class GameMode;

class PlayerController
{
public:
    PlayerController(Shape*, GameMode*);
    ~PlayerController() = default;

    void Check();
private:
    Shape* player;
    GameMode* game;
};