#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "GameMode/GameMode.h"
#include "Screen/Screen.h"
#include "Block/Block.h"
#include "Program/Program.h"

GameMode::GameMode():
    GameScreen(new Screen), prog(nullptr)
{}

GameMode::~GameMode()
{
    delete prog;
    delete GameScreen;
}

void GameMode::InitProgram()
{
    std::filesystem::path vert_path = std::filesystem::current_path();
    vert_path.append("Engine/Shaders/shader.vert");
    
    std::filesystem::path frag_path = std::filesystem::current_path();
    frag_path.append("Engine/Shaders/shader.frag");

    prog = new Program(vert_path, frag_path);
}


void GameMode::StartGame()
{
    GameLoop();
}

void GameMode::GameLoop()
{
    InitProgram();
    Block* triangle = new Block;
    GLFWwindow* window = GameScreen->GetWindow();

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog->GetProgram());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete triangle;
}