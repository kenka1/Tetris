#pragma once
#include "Type/Types.h"
#include "BaseActor/BaseActor.h"
#include "Actor/Actor.h"
#include "Shape/Shape.h"
#include "ObjectsData/BlockData.h"

#include <iostream>
#include <glm/glm.hpp>
#include <vector>

class Shape;

template<EForm Form>
class Actor: public BaseActor
{
public:
    Actor();
    virtual size_t GetSize() override;
    virtual Shape* operator[](size_t) override;
    virtual int16_t GetID() const override;

    static int16_t ID;
private:
    size_t size = 4;
    std::vector<Shape*> actors;
    EForm form = Form;

    Shape* InitializeDefaultActor();
    void Square();
    void Straight();
    void T();
    void L();
    void Skew();
};

template<EForm Form>
Actor<Form>::Actor():
    actors(size)
{
    ++ID;
    std::cout << "Constructor Actor" << std::endl;
    for(size_t i = 0; i < size; ++i)
        actors[i] = InitializeDefaultActor();
    if(form == EForm::Square)
    {
        Square();
    }
    if(form == EForm::Straight)
    {
        Straight();
    }
    if(form == EForm::T)
    {
        T();
    }
    if(form == EForm::L)
    {
        L();
    }
    if(form == EForm::Skew)
    {
        Skew();
    }

    for(size_t i = 0; i < size; ++i)
        actors[i]->UpdateTransform();
}

template<EForm Form>
void Actor<Form>::Square()
{
    actors[0]->Translate(glm::vec3(-50.0f, 500.0f, 0.0f));
    actors[1]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
    actors[2]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
    actors[3]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
}

template<EForm Form>
void Actor<Form>::Straight()
{
    actors[0]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
    actors[1]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
    actors[2]->Translate(glm::vec3(0.0f, 600.0f, 0.0f));
    actors[3]->Translate(glm::vec3(0.0f, 650.0f, 0.0f));
}

template<EForm Form>
void Actor<Form>::T()
{
    actors[0]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
    actors[1]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
    actors[2]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
    actors[3]->Translate(glm::vec3(50.0f, 550.0f, 0.0f));    
}

template<EForm Form>
void Actor<Form>::L()
{
    actors[0]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
    actors[1]->Translate(glm::vec3(-50.0f, 500.0f, 0.0f));
    actors[2]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
    actors[3]->Translate(glm::vec3(-50.0f, 600.0f, 0.0f));    
}

template<EForm Form>
void Actor<Form>::Skew()
{
    actors[0]->Translate(glm::vec3(0.0f, 500.0f, 0.0f));
    actors[1]->Translate(glm::vec3(0.0f, 550.0f, 0.0f));
    actors[2]->Translate(glm::vec3(-50.0f, 550.0f, 0.0f));
    actors[3]->Translate(glm::vec3(-50.0f, 600.0f, 0.0f));    
}

template<EForm Form>
int16_t Actor<Form>::ID = -1;

template<EForm Form>
size_t Actor<Form>::GetSize()
{
    return size;
}

template<EForm Form>
Shape* Actor<Form>::operator[](size_t index)
{
    return actors[index];
}

template<EForm Form>
int16_t Actor<Form>::GetID() const
{
    return ID;
}

template<EForm Form>
Shape* Actor<Form>::InitializeDefaultActor()
{
    Shape* cube = new Shape;
    cube->StoreData(sizeof(obj::data), obj::data);
    cube->StoreIndices(sizeof(obj::indices), obj::indices);
    cube->Scale(50.0f);
    cube->UpdateTransform();
    return cube;
}