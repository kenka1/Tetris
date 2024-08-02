#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "GameMode/GameMode.h"
#include "Screen/Screen.h"
#include "Shape/Shape.h"
#include "Program/Program.h"

GameMode::GameMode():
    GameScreen(new Screen), prog(nullptr), DeltaTime(0.0)
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
    Shape* triangle = new Shape;
    GLFWwindow* window = GameScreen->GetWindow();


    double LastFrame = 0.0;
    double CurrentFrame = 0.0;
    float FPS = 0.0;
    float temp_inc = 0.0;
    float temp_fps = 0.0;
    while(!glfwWindowShouldClose(window))
    {
        CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        if(temp_inc <= 1.0)
        {
            temp_inc += DeltaTime;
            ++temp_fps;
        }
        else
        {
            FPS = temp_fps;
            temp_inc = 0.0;
            temp_fps = 0.0;
        }

        std::cout << "DeltaTime :" << DeltaTime << " " << "FPS :" << FPS << std::endl;

        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog->GetProgram());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete triangle;
}