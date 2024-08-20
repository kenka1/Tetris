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
    std::vector<Shape*> actors;
    size_t size;
    EForm form = Form;

    Shape* InitializeDefaultActor();
};

template<EForm Form>
Actor<Form>::Actor():
    actors(4), size(4)
{
    ++ID;
    std::cout << "Constructor Actor" << std::endl;
    if(form == EForm::Cube)
    {
        for(size_t i = 0; i < 4; ++i)
            actors[i] = InitializeDefaultActor();

        actors[1]->Translate(glm::vec3(50.0f, 0.0f, 0.0f));
        actors[2]->Translate(glm::vec3(50.0f, 50.0f, 0.0f));
        actors[3]->Translate(glm::vec3(0.0f, 50.0f, 0.0f));

        for(size_t i = 0; i < 4; ++i)
            actors[i]->UpdateTransform();
    }

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