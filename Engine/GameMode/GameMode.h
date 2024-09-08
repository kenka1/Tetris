#pragma once
#include "Type/Types.h"

#include <vector>
#include <glm/glm.hpp>

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

    void ProcessInputs(); // implement ??
    void UpdateGame();
    void Render();

    void MoveEvent();
    void StepEvent();

    bool CanMove(const glm::vec3&);
    void Move(const glm::vec3&);
    void Rotate(); // refactoring too much code ??
    void StopMove();
    void EndGame(int16_t);
    bool CheckSideBound(const glm::vec3&);
    bool CheckSideBound();
    void CalculateDeltaTime(); // refactoring::implement ??

    void CreateNewPlayer();
};