#pragma once

#include <vector>

class Screen;
class GameState;
class Program;
class PlayerState;
class PlayerController;

class GameMode
{
public:
    GameMode();
    ~GameMode();
    void StartGame();

    inline GameState* GetGameState() {return _GameState;}
    inline PlayerController* GetPlayerController() {return _PlayerController;}
private:
    Screen* _GameScreen;
    PlayerController* _PlayerController;
    PlayerState* _PlayerState;
    GameState* _GameState;
    Program* _Program;
    double DeltaTime;

    void GameLoop();
    void Initialization();
    void Render();
    void CalculateDeltaTime(); // refactoring::implement
};