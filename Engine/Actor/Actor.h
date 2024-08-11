#pragma once
#include "Shape/Shape.h"

class Actor: public Shape
{
public:
    Actor();
    ~Actor() = default;

    void Tick(double DeltaTime);

private:
};