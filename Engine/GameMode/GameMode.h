#pragma once

class Screen;
class Program;

class GameMode
{
public:
    GameMode();
    ~GameMode();
    void StartGame();
private:
    Screen* GameScreen;
    Program* prog;
    double DeltaTime;
    void GameLoop();
    void InitProgram();
};