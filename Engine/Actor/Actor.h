#pragma once
#include "Type/Types.h"
#include "BaseActor/BaseActor.h"
#include "Actor/Actor.h"
#include "Shape/Shape.h"
#include "ObjectsData/BlockData.h"

#include <iostream>
#include <cstddef>
#include <glm/glm.hpp>
#include <vector>

class Shape;

template<EForm Form>
class Actor: public BaseActor
{
public:

    Actor(): actors(size)
    {
        // std::cout << "Constructor Actor" << std::endl;
        for(std::size_t i = 0; i < size; ++i)
            actors[i] = InitializeDefaultActor();
            
        switch (form)
        {
        case EForm::Square:
            Square();
            break;
        case EForm::Straight:
            Straight();
            states = 2;
            break;
        case EForm::T:
            states = 4;
            T();
            break;
        case EForm::L:
            states = 4;
            L();
            break;
        case EForm::Skew:
            states = 4;
            Skew();
            break;
        }

        for(size_t i = 0; i < size; ++i)
            actors[i]->UpdateTransform();
    }

    virtual std::size_t GetSize() override
    {
        return size;
    }

    virtual Shape* operator[](std::size_t index) override
    {
        return actors[index];
    }

    virtual void Rotate() override
    {
        switch(form)
        {
        case EForm::Straight:
            std::cout << "Rotate Straight" << std::endl;
            RotateStraight();
            break;
        case EForm::T:
            std::cout << "Rotate T" << std::endl;
            RotateT();
            break;
        case EForm::L:
            std::cout << "Rotate L" << std::endl;
            RotateL();
            break;
        case EForm::Skew:
            std::cout << "Rotate Skew" << std::endl;
            RotateSkew();
            break;
        default:
            break;
        }
    }

    virtual void ChangeCurrentState() override
    {
        current_state = (current_state + 1) % states;
    }
private:
    std::size_t size = 4;
    int8_t states = 0;
    int8_t current_state = 0;
    std::vector<Shape*> actors;
    EForm form = Form;

    Shape* InitializeDefaultActor()
    {
        Shape* cube = new Shape;
        cube->StoreData(sizeof(obj::data), obj::data);
        cube->StoreIndices(sizeof(obj::indices), obj::indices);
        cube->Scale(50.0f);
        cube->UpdateTransform();
        return cube;
    }

    void Square()
    {
        actors[0]->Translate(glm::vec3(-50.0f, 500.0f, 0.0f));
        actors[1]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
        actors[2]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
        actors[3]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
    }

    void Straight()
    {
        actors[1]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
        actors[0]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
        actors[2]->Translate(glm::vec3(0.0f, 600.0f, 0.0f));
        actors[3]->Translate(glm::vec3(0.0f, 650.0f, 0.0f));
    }

    void T()
    {
        actors[1]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
        actors[0]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
        actors[2]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
        actors[3]->Translate(glm::vec3(50.0f, 550.0f, 0.0f));  
    }

    void L()
    {
        actors[1]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
        actors[2]->Translate(glm::vec3(-50.0f, 500.0f, 0.0f));
        actors[0]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
        actors[3]->Translate(glm::vec3(-50.0f, 600.0f, 0.0f));    
    }

    void Skew()
    {
        actors[1]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
        actors[0]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
        actors[2]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
        actors[3]->Translate(glm::vec3(-50.0f, 600.0f, 0.0f));    
    }

    void RotateStraight()
    {
        switch (current_state)
        {
        case 0:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(100.0f, -100.0f, 0.0f));
            break;
        case 1:
            actors[1]->Translate(actors[1]->GetTranslate() - glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() - glm::vec3(50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() - glm::vec3(100.0f, -100.0f, 0.0f));
            break;
        }
    }

    void RotateT()
    {
        switch (current_state)
        {
        case 0:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            break;
        case 1:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            break;
        case 2:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            break;
        case 3:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            break;
        }
    }

    void RotateL()
    {
        switch (current_state)
        {
        case 0:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(-100.0f, 0.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            break;
        case 1:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(0.0f, 100.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            break;
        case 2:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(100.0f, 0.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            break;
        case 3:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(0.0f, -100.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            break;
        }
    }

    void RotateSkew()
    {
        switch (current_state)
        {
        case 0:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(100.0f, 0.0f, 0.0f));
            break;
        case 1:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(50.0f, 50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(0.0f, -100.0f, 0.0f));
            break;
        case 2:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(50.0f, -50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(-100.0f, 0.0f, 0.0f));
            break;
        case 3:
            actors[1]->Translate(actors[1]->GetTranslate() + glm::vec3(-50.0f, -50.0f, 0.0f));
            actors[2]->Translate(actors[2]->GetTranslate() + glm::vec3(-50.0f, 50.0f, 0.0f));
            actors[3]->Translate(actors[3]->GetTranslate() + glm::vec3(0.0f, 100.0f, 0.0f));
            break;
        }
    }
};