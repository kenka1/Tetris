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
    _GameState(new GameState),_Program(nullptr)
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
        glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(_Program->GetProgram());

        // Grid Render Start
        glBindVertexArray(Grid->GetVao());
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "isTexture"), 0);
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                    1, GL_FALSE, &_GameScreen->GetProjection()[0][0]);
        glUniform4f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.55f, 0.66f, 0.7f, 1.0f);
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "uGrid_ID"), 0);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 9);

        glBindVertexArray(Grid2->GetVao());

        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "isTexture"), 0);
        glLineWidth(3.0f);
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "uGrid_ID"), 1);
        glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, nullptr, 19);
        // Grid Render end

        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete Grid2;
    delete Grid;
}

void GameMode::ProcessInputs()
{
    if(_GameScreen->CanSpace == false)
    {
        TimeData::time_space += TimeData::DeltaTime;

        if(TimeData::time_space >= 0.35f)
        {
            _GameScreen->CanSpace = true;
            TimeData::time_space = 0.0f;
        }
    }
}

void GameMode::UpdateGame()
{
    CalculateDeltaTime();

    // Move
    if(_PlayerController->GetPlayer() != nullptr && _PlayerState->GetStop() != true)
    {
        if(TimeData::time_move <= 0.1f)
        {
            TimeData::time_move += TimeData::DeltaTime;
        }
        else
        {                
            MoveEvent();
            TimeData::time_move = 0.0f;
        }
        if(TimeData::time_step <= TimeData::MaxTime_step)
        {
            TimeData::time_step += TimeData::DeltaTime;
        }
        else
        {
            StepEvent();
            TimeData::time_step = 0.0f;
        }
        TimeData::MaxTime_step -= TimeData::MaxTime_step_delta;
        TimeData::MaxTime_step = std::max(TimeData::MaxTime_step, TimeData::MaxTime_step);
    }
    else if (_GameState->GetDelay() != true)
    {
        CreateNewPlayer();
    }

    // Step after remove line
    if(_GameState->GetDelay() == true)
    {
        if(TimeData::time_move_line <= 0.5f)
        {
            TimeData::time_move_line += TimeData::DeltaTime;
        }
        else
        {
            _GameState->MoveLine();
            TimeData::time_move_line = 0.0f;
        }
    }
}

void GameMode::CalculateDeltaTime()
{
    TimeData::CurrentFrame = glfwGetTime();
    TimeData::DeltaTime = TimeData::CurrentFrame - TimeData::LastFrame;
    TimeData::LastFrame = TimeData::CurrentFrame;
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
        if(call == 6)
        {
            MoveToStop();
            return;
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
    CalculatePredict();

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
        _GameState->AddToGrid(target, Grid_ID, _PlayerState->Player_ID, player->GetType());
    }
}

void GameMode::CalculatePredict()
{
    glm::vec3 offset(0.0f, -50.0f, 0.0f);
    while(CanMove(offset))
    {
        offset.y -= 50.0f;
    }
    offset.y += 50.0f;
    std::cout << "Offset : " << offset.y << std::endl;
    BaseActor* player = _PlayerController->GetPlayer();
    std::vector<glm::vec3> offsets(4);
    for(size_t i = 0; i < 4; ++i)
    {
        Shape* target = (*player)[i];
        glm::vec3 pos = target->GetTranslate();
        std::cout << "Player pos : " << pos.y << std::endl;
        offsets[i] = pos + offset;
    }

    _GameState->UpdatePredictGrid(offsets);
}

void GameMode::MoveToStop()
{
    glm::vec3 offset(0.0f, -50.0f, 0.0f);
    while(CanMove(offset))
    {
        offset.y -= 50.0f;
    }
    offset.y += 50.0f;
    std::cout << "offset : " << offset.y << std::endl;

    Move(offset);
    StopMove();
    _GameScreen->CanSpace = false;
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
            _GameState->AddToGrid(target, prev_data[i].second, _PlayerState->Player_ID, player->GetType());
            target->UpdateTransform();
        }
        CalculatePredict();
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
        _GameState->AddToGrid((*player)[i], Grid_ID, _PlayerState->Player_ID, rand);
    }
    CalculatePredict();
}

void GameMode::Render()
{
    std::vector<PlayerInfo>& Grid = _GameState->GetGrid();
    for(size_t i = 0; i < Grid.size(); ++i)
    {
        if(Grid[i].Target != nullptr)
        {
            Shape* player = Grid[i].Target;
            float color[3];
            switch (Grid[i].type)
            {
            case EForm::Square:
                color[0] = 0.4f; color[1] = 0.8f;; color[2] = 0.8f;
                break;
            case EForm::Straight:
                color[0] = 0.9f; color[1] = 0.9f; color[2] = 0.4f;
                break;
            case EForm::T:
                color[0] = 0.6f; color[1] = 0.4f; color[2] = 0.6f;
                break;
            case EForm::L:
                color[0] = 0.4f; color[1] = 0.8f; color[2] = 0.4f;
                break;
            case EForm::Skew:
                color[0] = 0.8f; color[1] = 0.4f; color[2] = 0.4f;
                break;
            }
            glm::mat4 Transform = _GameScreen->GetProjection() * player->GetTransform();
            glBindVertexArray(player->GetVao());
            int isTexture = 0;
            if(player->GetTexture() != 111)
            {
                isTexture = 1;
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, player->GetTexture());
            }
            glUniform1i(glGetUniformLocation(_Program->GetProgram(), "isTexture"), isTexture);
            glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                        1, GL_FALSE, &Transform[0][0]);
            glUniform4f(glGetUniformLocation(_Program->GetProgram(), "uColor"), color[0], color[1], color[2], 1.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }
    }

    std::vector<Shape*> PredictGrid = _GameState->GetPredictGrid();
    for(size_t i = 0; i < 4; ++i)
    {
        Shape* player = PredictGrid[i];
        glm::mat4 Transform = _GameScreen->GetProjection() * player->GetTransform();
        glBindVertexArray(player->GetVao());
        int isTexture = 0;
        if(player->GetTexture() != 111)
        {
            isTexture = 1;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, player->GetTexture());
        }
        glUniform1i(glGetUniformLocation(_Program->GetProgram(), "isTexture"), isTexture);
        glUniformMatrix4fv(glGetUniformLocation(_Program->GetProgram(), "uTransform"), 
                                                    1, GL_FALSE, &Transform[0][0]);
        glUniform4f(glGetUniformLocation(_Program->GetProgram(), "uColor"), 0.96f, 0.54f, 0.3f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}