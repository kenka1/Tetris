#pragma once
#include "Shape/Shape.h"
#include <glm/glm.hpp>

class Actor: public Shape
{
public:
    Actor();

    void Tick(double DeltaTime);
    glm::vec3 Move(int8_t);
private:
};