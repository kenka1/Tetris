#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "GameMode/GameMode.h"
#include "Screen/Screen.h"
#include "GameState/GameState.h"
#include "PlayerState/PlayerState.h"
#include "PlayerController/PlayerController.h"
#include "BaseActor/BaseActor.h"
#include "Actor/Actor.h"
#include "Shape/Shape.h"
#include "Program/Program.h"

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
    Shape* Grid = new Shape;
    Grid->StoreData(sizeof(grid::data), grid::data);
    Grid->StoreIndices(sizeof(grid::indices), grid::indices);

    Shape* Grid2 = new Shape;
    Grid2->StoreData(sizeof(grid::data2), grid::data2);
    Grid2->StoreIndices(sizeof(grid::indices), grid::indices);

    GLFWwindow* window = _GameScreen->GetWindow();
    BaseActor* player = nullptr;

    glm::mat4 Proj = _GameScreen->GetProjection();

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
            if(time_move <= 0.1f)
            {
                time_move += DeltaTime;
            }
            else
            {                
                MoveEvent(player);
                time_move = 0.0f;
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

        Render();

        // Grid Render Start
        glBindVertexArray(Grid->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                    1, GL_FALSE, &Proj[0][0]);
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

// FIX 3x LOOPS !!!!
void GameMode::MoveEvent(BaseActor* player)
{
    int8_t call = _GameScreen->Move();
    if(call != -1)
    {
        //DEBUG
        {
            std::vector<PlayerInfo> Grid = _GameState->GetGrid();
            for(int i = 0; i < Grid.size(); ++i)
            {
                if(Grid[i].Target != nullptr)
                    std::cout << "busy : " << i << std::endl;
            }
        }


        glm::vec3 Offset = _PlayerController->Move(call);
        bool IsMoving = true;
        size_t N = player->GetSize();
        int16_t Player_ID = _PlayerState->GetPlayerID();
        std::cout << "Player_ID : " << Player_ID << std::endl;

        for(size_t i = 0; i < N; ++i)
        {   
            Shape* target = (*player)[i];
            glm::vec3 pos = target->GetTranslate();
            int16_t Grid_ID = _PlayerState->CalculateID(pos + Offset);

            std::cout << "Grid ID : " << Grid_ID << std::endl;
            if(_GameState->CheckCell(Grid_ID, Player_ID))
                IsMoving = false;
        }

        if(IsMoving)
        {
            for(size_t i = 0; i < N; ++i)
            {
                int16_t Old_ID = _PlayerState->GetID(i);
                _GameState->ClearGrid(Old_ID);
            }

            for(size_t i = 0; i < N; ++i)
            {
                Shape* target = (*player)[i];
                glm::vec3 pos = target->GetTranslate() + Offset;
                int16_t Grid_ID = _PlayerState->CalculateID(pos);

                target->Translate(pos);
                target->UpdateTransform();
                _PlayerState->SetID(i, Grid_ID);
                _GameState->AddToGrid(target, Grid_ID, Player_ID);
            }
        }
        else
        {
            for(size_t i = 0; i < N; ++i)
            {
                int16_t Grid_ID = _PlayerState->GetID(i);

                _PlayerState->SetStop(true);
                _GameState->RemoveLine(Grid_ID);
            }
        }
    }
}

void GameMode::CreateNewPlayer(BaseActor*& player)
{
    player = new Actor<EForm::Cube>();
    int16_t Player_ID = player->GetID();

    _PlayerController->SetPlayer(player);
    _PlayerState->SetStop(false);
    _PlayerState->SetPlayerID(Player_ID);

    for(size_t i = 0; i < player->GetSize(); ++i)
    {
        int16_t Grid_ID = _PlayerState->CalculateID((*player)[i]->GetTranslate());
        _PlayerState->SetID(i, Grid_ID);
        _GameState->AddToGrid((*player)[i], Grid_ID, Player_ID);
    }
}

void GameMode::Render()
{
    std::vector<PlayerInfo>& Grid = _GameState->GetGrid();
    for(size_t i = 0; i < Grid.size(); ++i)
    {
        if(Grid[i].Target != nullptr)
        {
            Shape* player = Grid[i].Target;
            glm::mat4 Transform = _GameScreen->GetProjection() * player->GetTransform();
            glBindVertexArray(player->GetVao());
            glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                        1, GL_FALSE, &Transform[0][0]);
            glUniform3f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.7f, 0.3f, 0.2f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }
    }
}