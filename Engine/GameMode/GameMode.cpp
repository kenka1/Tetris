#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "GameMode/GameMode.h"
#include "Screen/Screen.h"
#include "GameState/GameState.h"
#include "PlayerState/PlayerState.h"
#include "PlayerController/PlayerController.h"
#include "Actor/Actor.h"
#include "Program/Program.h"

#include "ObjectsData/BlockData.h"
#include "ObjectsData/GridData.h"

GameMode::GameMode():
    _GameScreen(new Screen), _PlayerController(new PlayerController(this)), 
    _PlayerState(new PlayerState(this)),
    _GameState(new GameState),_Program(nullptr), DeltaTime(0.0)
{
    std::cout << "Contructor GameMode" << std::endl;
}

GameMode::~GameMode()
{
    delete _Program;
    delete _PlayerController;
    delete _PlayerState;
    delete _GameState;
    delete _GameScreen;
}

void GameMode::Initialization()
{
    // Initialize Actor
    Actor* block = new Actor;
    block->StoreData(sizeof(obj::data), obj::data);
    block->StoreIndices(sizeof(obj::indices), obj::indices);
    block->Scale(50.0f);
    block->UpdateTransform();

    // Initialize PlayerController
    _PlayerController->SetPlayer(block);

    // Initialize PlayerState
    _PlayerState->CalculateID(_PlayerController->GetPlayer());

    // Initialize GameState


    // Initialize program
    std::filesystem::path vert_path = std::filesystem::current_path();
    vert_path.append("Engine/Shaders/shader.vert");
    
    std::filesystem::path frag_path = std::filesystem::current_path();
    frag_path.append("Engine/Shaders/shader.frag");

    _Program = new Program(vert_path, frag_path);

    
}


void GameMode::StartGame()
{
    Initialization();

    GameLoop();
}

void GameMode::GameLoop()
{
    //INITIALIZATION
    Actor* Grid = new Actor;
    Grid->StoreData(sizeof(grid::data), grid::data);
    Grid->StoreIndices(sizeof(grid::indices), grid::indices);

    Actor* Grid2 = new Actor;
    Grid2->StoreData(sizeof(grid::data2), grid::data2);
    Grid2->StoreIndices(sizeof(grid::indices), grid::indices);

    GLFWwindow* window = _GameScreen->GetWindow();

    glm::mat4 Proj = _GameScreen->GetProjection();
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

        //GAME UPDATE
        Actor* player = _PlayerController->GetPlayer();
        // make for block individually
        if(time_move <= 0.1f)
        {
            time_move += DeltaTime;
        }
        else
        {
            _GameScreen->callMove(player);
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

            CurrentPos = player->GetTranslate();
            CurrentPos.y -= 50.0f;
            // block->Translate(CurrentPos);
            // block->UpdateTransform();
            std::cout << "MOVE" << std::endl;
        }

        // RENDERING
        // std::cout << "DeltaTime :" << DeltaTime << " " << "FPS :" << FPS << std::endl;
        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(_Program->GetProgram());
        Model = player->GetTransform();
        Transform = Proj * Model;
        glBindVertexArray(player->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "Transform"), 
                                                    1, GL_FALSE, &Transform[0][0]);
        glUniform3f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.7f, 0.3f, 0.2f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(Grid->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "Transform"), 
                                                    1, GL_FALSE, &StaticTransform[0][0]);
        glUniform3f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.65f, 0.65f, 0.65f);
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "grid_ID"), 0);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 9);

        glBindVertexArray(Grid2->GetVao());
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "grid_ID"), 1);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 19);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete Grid2;
    delete Grid;
}