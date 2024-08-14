#include <iostream>

#include "Actor/Actor.h"

Actor::Actor()
{
    std::cout << "Constructor Actor" << std::endl;
}

void Actor::Tick(double DeltaTime)
{

}

glm::vec3 Actor::Move(int8_t call)
{
    float step = 50.0f;
    glm::vec3 pos = GetTranslate();
    switch(call)
    {
    case 0:
        pos.x += step;
        return pos;
    case 1:
        pos.x -= step;
        return pos;
    case 2:
        pos.y -= step;
        return pos;
    case 3:
        pos.y += step;
        return pos;
    case 4:
        pos.x += step;
        pos.y -= step;
        return pos;
    case 5:
        pos.x -= step;
        pos.y -= step;
        return pos;
    }
}