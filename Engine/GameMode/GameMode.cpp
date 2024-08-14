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
    // Actor* block = new Actor;
    // block->StoreData(sizeof(obj::data), obj::data);
    // block->StoreIndices(sizeof(obj::indices), obj::indices);
    // block->Scale(50.0f);
    // block->UpdateTransform();

    // Initialize PlayerController
    // _PlayerController->SetPlayer(block);

    // Initialize PlayerState
    // int16_t id = _PlayerState->CalculateID(block->GetTranslate());


    // Initialize GameState
    // _GameState->AddToGrid(block, id);

    // Initialize program
    std::filesystem::path vert_path = std::filesystem::current_path();
    vert_path.append("Engine/Shaders/shader.vert");
    
    std::filesystem::path frag_path = std::filesystem::current_path();
    frag_path.append("Engine/Shaders/shader.frag");

    _Program = new Program(vert_path, frag_path);

    
}

void GameMode::CreateNewPlayer(Actor*& player)
{
    std::cout << "Creat new actor" << std::endl;
    player = new Actor;
    player->StoreData(sizeof(obj::data), obj::data);
    player->StoreIndices(sizeof(obj::indices), obj::indices);
    player->Scale(50.0f);
    player->UpdateTransform();

    _PlayerController->SetPlayer(player);
    _PlayerState->SetStop(false);
    int16_t id = _PlayerState->CalculateID(player->GetTranslate());
    _PlayerState->SetID(id);
    _GameState->AddToGrid(player, id);
}

void GameMode::Render()
{
    std::vector<Actor*>& Grid = _GameState->GetGrid();
    for(size_t i = 0; i < Grid.size(); ++i)
    {
        if(Grid[i] != nullptr)
        {
            Actor* player = Grid[i];
            glm::mat4 Transform = _GameScreen->GetProjection() * player->GetTransform();
            glBindVertexArray(player->GetVao());
            glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                        1, GL_FALSE, &Transform[0][0]);
            glUniform3f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.7f, 0.3f, 0.2f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }
    }
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
    Actor* player = nullptr;

    glm::mat4 Proj = _GameScreen->GetProjection();
    glm::mat4 Model(1.0f);
    glm::mat4 Transform(1.0f);
    glm::mat4 StaticTransform(Proj);
    glm::vec3 CurrentPos(0.0f);

    double LastFrame = 0.0;
    double CurrentFrame = 0.0;
    // float FPS = 0.0;
    // float time = 0.0;
    float time_move = 0.0;
    // float temp_fps = 0.0;
    while(!glfwWindowShouldClose(window))
    {
        // Calculate DeltaTime
        CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        if(player == nullptr)
            std::cout << "PLAYER IS NULLPTR" << std::endl;

        // Game update
        if(player != nullptr && _PlayerState->GetStop() != true)
        {
            glm::vec3 Offset(0.0f);
            if(time_move <= 0.1f)
            {
                time_move += DeltaTime;
            }
            else
            {
                int8_t call = _GameScreen->Move();
                time_move = 0.0f;
                if(call != -1)
                {
                    Offset = player->Move(call);
                    int16_t id = _PlayerState->CalculateID(Offset);
                    std::cout << "next ID : " << id << std::endl;
                    if(_GameState->CheckCell(id))
                    {
                        _GameState->AddToGrid(player, _PlayerState->GetID());
                        _PlayerState->SetStop(true);
                    }
                    else
                    {
                        player->Translate(Offset);
                        _GameState->AddToGrid(nullptr, _PlayerState->GetID());
                        _PlayerState->SetID(id);
                        _GameState->AddToGrid(player, id);
                        player->UpdateTransform();
                    }
                    std::cout << "ID : " << _PlayerState->GetID() << std::endl;
                }
            }
        }
        else
        {
            CreateNewPlayer(player);
        }

            // if(time <= 1.0f)
            // {
            //     time += DeltaTime;
            //     ++temp_fps;
            // }
            // else
            // {
            //     FPS = temp_fps;
            //     time = 0.0;
            //     temp_fps = 0.0;

            //     CurrentPos = player->GetTranslate();
            //     CurrentPos.y -= 50.0f;
            //     // block->Translate(CurrentPos);
            //     // block->UpdateTransform();
            //     std::cout << "MOVE" << std::endl;
            // }


        // RENDERING
        // std::cout << "DeltaTime :" << DeltaTime << " " << "FPS :" << FPS << std::endl;
        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(_Program->GetProgram());

        // Model = player->GetTransform();
        // Transform = Proj * Model;
        // glBindVertexArray(player->GetVao());
        // glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "Transform"), 
        //                                             1, GL_FALSE, &Transform[0][0]);
        // glUniform3f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.7f, 0.3f, 0.2f);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Render all objects in scene
        Render();

        // Grid Render Start
        glBindVertexArray(Grid->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                    1, GL_FALSE, &StaticTransform[0][0]);
        glUniform3f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.65f, 0.65f, 0.65f);
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "uGrid_ID"), 0);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 9);

        glBindVertexArray(Grid2->GetVao());
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "uGrid_ID"), 1);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 19);
        // Grid Render end

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete Grid2;
    delete Grid;
}