#pragma once

#include <vector>

class Screen;
class GameState;
class Program;
class Shape;

class GameMode
{
public:
    GameMode();
    ~GameMode();
    void StartGame();
private:
    Screen* GameScreen;
    GameState* States;
    Program* prog;
    double DeltaTime;
    std::vector<Shape*> Buffer;

    void GameLoop();
    void InitProgram();
    void CalculateDeltaTime(); // refactoring::implement
};