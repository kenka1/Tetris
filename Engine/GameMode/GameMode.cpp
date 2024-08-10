#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "GameMode/GameMode.h"
#include "Screen/Screen.h"
#include "GameState/GameState.h"
#include "Shape/Shape.h"
#include "Program/Program.h"

#include "ObjectsData/BlockData.h"
#include "ObjectsData/GridData.h"

GameMode::GameMode():
    GameScreen(new Screen), prog(nullptr), DeltaTime(0.0)
{
    std::cout << "Contructor GameMode" << std::endl;
}

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

    Shape* block = new Shape;
    block->StoreData(sizeof(obj::data), obj::data);
    block->StoreIndices(sizeof(obj::indices), obj::indices);
    block->Scale(50.0f);
    block->UpdateTransform();

    Shape* Grid = new Shape;
    Grid->StoreData(sizeof(grid::data), grid::data);
    Grid->StoreIndices(sizeof(grid::indices), grid::indices);

    Shape* Grid2 = new Shape;
    Grid2->StoreData(sizeof(grid::data2), grid::data2);
    Grid2->StoreIndices(sizeof(grid::indices), grid::indices);

    GLFWwindow* window = GameScreen->GetWindow();

    glm::mat4 Proj = GameScreen->GetProjection();
    glm::mat4 Model(1.0f);
    glm::mat4 Transform(1.0f);
    glm::mat4 StaticTransform(Proj);
    glm::vec3 CurrentPos(0.0f);

    double LastFrame = 0.0;
    double CurrentFrame = 0.0;
    float FPS = 0.0;
    float time = 0.0;
    float time_move = 0.0;
    float temp_fps = 0.0;
    while(!glfwWindowShouldClose(window))
    {
        CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        // make for block individually
        if(time_move <= 0.1f)
        {
            time_move += DeltaTime;
        }
        else
        {
            GameScreen->callMove(block);
            time_move = 0.0f;
        }

        if(time <= 1.0f)
        {
            time += DeltaTime;
            ++temp_fps;
        }
        else
        {
            FPS = temp_fps;
            time = 0.0;
            temp_fps = 0.0;

            CurrentPos = block->GetTranslate();
            CurrentPos.y -= 50.0f;
            // block->Translate(CurrentPos);
            // block->UpdateTransform();
            std::cout << "MOVE" << std::endl;
        }

        // std::cout << "DeltaTime :" << DeltaTime << " " << "FPS :" << FPS << std::endl;
        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog->GetProgram());
        Model = block->GetTransform();
        Transform = Proj * Model;
        glBindVertexArray(block->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(prog->GetProgram(), "Transform"), 
                                                    1, GL_FALSE, &Transform[0][0]);
        glUniform3f(glGetUniformLocation(prog->GetProgram(), "uColor"), 0.7f, 0.3f, 0.2f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(Grid->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(prog->GetProgram(), "Transform"), 
                                                    1, GL_FALSE, &StaticTransform[0][0]);
        glUniform3f(glGetUniformLocation(prog->GetProgram(), "uColor"), 0.65f, 0.65f, 0.65f);
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(prog->GetProgram(), "grid_ID"), 0);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 9);

        glBindVertexArray(Grid2->GetVao());
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(prog->GetProgram(), "grid_ID"), 1);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 19);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete block;
}