#include <iostream>
#include <random>

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
#include "GameStatics/GameStatics.h"

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
    
    while(!glfwWindowShouldClose(window))
    {
        ProcessInputs();

        UpdateGame();

        // RENDERING
        glClearColor(0.3f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(_Program->GetProgram());

        Render();

        // Grid Render Start
        glBindVertexArray(Grid->GetVao());
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                    1, GL_FALSE, &_GameScreen->GetProjection()[0][0]);
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

void GameMode::ProcessInputs()
{

}

void GameMode::UpdateGame()
{
    // Calculate DeltaTime
    TimeData::CurrentFrame = glfwGetTime();
    DeltaTime = TimeData::CurrentFrame - TimeData::LastFrame;
    TimeData::LastFrame = TimeData::CurrentFrame;

    // Game update
    if(_PlayerController->GetPlayer() != nullptr && _PlayerState->GetStop() != true)
    {
        if(TimeData::time_move <= 0.1f)
        {
            TimeData::time_move += DeltaTime;
        }
        else
        {                
            MoveEvent();
            TimeData::time_move = 0.0f;
        }
        if(TimeData::time_step <= 0.2f)
        {
            TimeData::time_step += DeltaTime;
        }
        else
        {
            StepEvent();
            TimeData::time_step = 0.0f;
        }
    }
    else if (_GameState->GetDelay() != true)
    {
        CreateNewPlayer();
    }

    // Step after remove line
    if(_GameState->GetDelay() == true)
    {
        if(time_move_line <= 0.5f)
        {
            time_move_line += DeltaTime;
        }
        else
        {
            _GameState->MoveLine();
            time_move_line = 0.0f;
        }
    }
}

void GameMode::MoveEvent()
{
    int8_t call = _GameScreen->Move();
    if(call != -1)
    {
        if(call == 3)
        {
            if(_GameScreen->CanRotate)
            {
                Rotate();
                _GameScreen->CanRotate = false;
            }
            return; // can we press 2 buttons ???
        }
        glm::vec3 offset = _PlayerController->Move(call);
        bool IsMoving = true;
        if(call == 2 || call == 4 || call == 5)
            CanMove(offset);

        if(IsMoving)
        {
            Move(offset);
        }
        else
        {
            StopMove();
        }

        //Debug
        std::cout << "---------------------------------------------------" << std::endl;
        std::vector<PlayerInfo>& Grid = _GameState->GetGrid();
        for(size_t i = 0; i < Grid.size(); ++i)
        {
            if(Grid[i].Target != nullptr)
            {
                std::cout << "i : " << i << " ";
                std::cout << "Target : " << Grid[i].Target << " ";
                std::cout << "ID : " << Grid[i].Player_ID << " ";
                std::cout << "Stop : " << Grid[i].stop << std::endl;
            }
        }
        std::cout << "---------------------------------------------------" << std::endl;
        //Debug
    }
    _GameScreen->CanRotate = true;
}

void GameMode::StepEvent()
{
    glm::vec3 offset(0.0f, -50.0f, 0.0f);
    bool IsMoving = CanMove(offset);

    if(IsMoving)
    {
        Move(offset);
    }
    else
    {
        StopMove();
    }
}

bool GameMode::CanMove(const glm::vec3& offset)
{
    BaseActor* player = _PlayerController->GetPlayer();
    size_t N = player->GetSize();

    bool IsMoving = true;
    for(size_t i = 0; i < N; ++i)
    {   
        Shape* target = (*player)[i];
        glm::vec3 pos = target->GetTranslate();
        int16_t Grid_ID = _PlayerState->CalculateID(pos + offset);
        if(_GameState->CheckCell(Grid_ID, _PlayerState->Player_ID))
            IsMoving = false;
    }

    return IsMoving;
}

void GameMode::Move(const glm::vec3& offset)
{
    if(CheckSideBound(offset) || !CanMove(offset))
        return;

    BaseActor* player = _PlayerController->GetPlayer();
    size_t N = player->GetSize();

    for(size_t i = 0; i < N; ++i)
    {
        int16_t Old_ID = _PlayerState->GetID(i);
        _GameState->ClearGrid(Old_ID);
    }

    for(size_t i = 0; i < N; ++i)
    {
        Shape* target = (*player)[i];
        glm::vec3 pos = target->GetTranslate() + offset;
        int16_t Grid_ID = _PlayerState->CalculateID(pos);

        target->Translate(pos);
        target->UpdateTransform();
        _PlayerState->SetID(i, Grid_ID);
        _GameState->AddToGrid(target, Grid_ID, _PlayerState->Player_ID);
    }
}

void GameMode::Rotate()
{
    std::cout << "Rotate event" << std::endl;
    BaseActor* player = _PlayerController->GetPlayer();
    if(dynamic_cast<Actor<EForm::Square>*>(player))
    {
        std::cout << "this is square, return" << std::endl;
        return;
    }
    size_t N = player->GetSize();

    // store prev data if we need to turn back
    std::vector<std::pair<glm::vec3, int16_t>> prev_data(N);
    for(size_t i = 0; i < N; ++i)
    {
        prev_data[i].first = (*player)[i]->GetTranslate();
    }

    player->Rotate();
    bool CanRotate = true;

    //debug
    std::cout << "Current ID" << std::endl;
    for(size_t i = 0; i < N; ++i)
    {
        std::cout << _PlayerState->GetID(i) << '\t';
    }
    std::cout << std::endl;
    //debug

    for(size_t i = 0; i < N; ++i)
    {
        Shape* target = (*player)[i];
        int16_t Grid_ID = _PlayerState->CalculateID(target->GetTranslate());
        std::cout << Grid_ID << '\t';
        prev_data[i].second = Grid_ID;
        if(_GameState->CheckCell(Grid_ID, _PlayerState->Player_ID) || CheckSideBound())
            CanRotate = false;
    }
    std::cout << CanRotate << std::endl;
    if(CanRotate)
    {
        player->ChangeCurrentState();
        for(size_t i = 0; i < N; ++i)
        {
            _GameState->ClearGrid(_PlayerState->GetID(i));
        }
        std::cout << "set new" << std::endl;
        for(size_t i = 0; i < N; ++i)
        {
            Shape* target = (*player)[i];
            _PlayerState->SetID(i, prev_data[i].second);
            _GameState->AddToGrid(target, prev_data[i].second, _PlayerState->Player_ID);
            target->UpdateTransform();
        }
    }
    else
    {
        for(size_t i = 0; i < N; ++i)
        {
            Shape* target = (*player)[i];
            target->Translate(prev_data[i].first);
        }
    }
}

void GameMode::StopMove()
{
    BaseActor* player = _PlayerController->GetPlayer();
    size_t N = player->GetSize();
    for(size_t i = 0; i < N; ++i)
    {
        int16_t Grid_ID = _PlayerState->GetID(i);

        _PlayerState->SetStop(true);
        _GameState->GetGrid()[Grid_ID].stop = true;
        EndGame(Grid_ID);
        _GameState->RemoveLine(Grid_ID);
    }
}

void GameMode::EndGame(int16_t Grid_ID)
{
    if(_PlayerState->GetStop() == true && Grid_ID >= 200)
        glfwSetWindowShouldClose(_GameScreen->GetWindow(), 1);
}

bool GameMode::CheckSideBound(const glm::vec3& offset)
{
    BaseActor* player = _PlayerController->GetPlayer();
    size_t N = player->GetSize();

    for(size_t i = 0; i < N; ++i)
    {
        Shape* target = (*player)[i];
        glm::vec3 pos = target->GetTranslate() + offset;
        if(pos.x < -250.0 || pos.x > 200)
            return true;
    }
    return false;
}

bool GameMode::CheckSideBound()
{
    BaseActor* player = _PlayerController->GetPlayer();
    size_t N = player->GetSize();

    for(size_t i = 0; i < N; ++i)
    {
        Shape* target = (*player)[i];
        glm::vec3 pos = target->GetTranslate();
        if(pos.x < -250.0 || pos.x > 200)
            return true;
    }
    return false;
}

void GameMode::CreateNewPlayer()
{
    std::vector<EForm>& DrawR = _GameState->Draw;
    std::vector<EForm>& QueueR = _GameState->Queue;
    if(DrawR.size() == 0)
    {
        _GameState->ResetDraw();
    }

    int max = static_cast<int>(DrawR.size()) - 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(0, max);
    int index = dis(gen);
    EForm rand = DrawR[index];

    DrawR.erase(DrawR.begin() + index);
    QueueR.push_back(rand);

    BaseActor* player = nullptr;
    switch(rand)
    {
    case EForm::Square:
       player = new Actor<EForm::Square>();
       break;
    case EForm::Straight:
        player = new Actor<EForm::Straight>();
        break;
    case EForm::T:
        player = new Actor<EForm::T>();
        break;
    case EForm::L:
        player = new Actor<EForm::L>();
        break;
    case EForm::Skew:
        player = new Actor<EForm::Skew>();
        break;
    }

    _PlayerController->SetPlayer(player);
    _PlayerState->SetStop(false);
    ++_PlayerState->Player_ID;

    for(size_t i = 0; i < player->GetSize(); ++i)
    {
        int16_t Grid_ID = _PlayerState->CalculateID((*player)[i]->GetTranslate());
        _PlayerState->SetID(i, Grid_ID);
        _GameState->AddToGrid((*player)[i], Grid_ID, _PlayerState->Player_ID);
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