#pragma once
#include "Type/Types.h"

#include <vector>

class Screen;
class GameState;
class Program;
class PlayerState;
class PlayerController;
class BaseActor;
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

    void Initialization();
    void GameLoop();

    void MoveEvent(BaseActor* player);
    void CreateNewPlayer(BaseActor*&);
    void Render();
    void CalculateDeltaTime(); // refactoring::implement
};