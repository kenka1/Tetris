#pragma once
#include "Shape/Shape.h"

class Actor: public Shape
{
public:
    Actor();

    void Tick(double DeltaTime);

private:
};