#pragma once

#include "Type/Types.h"

#include <cstddef>
#include <cstdint>

class Shape;

class BaseActor
{
public:
    virtual ~BaseActor() = default;
    virtual size_t GetSize() = 0;
    virtual Shape* operator[](size_t) = 0;
    virtual void Rotate() = 0;
    virtual void ChangeCurrentState() = 0;
    virtual EForm GetType() = 0;
};