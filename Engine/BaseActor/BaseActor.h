#pragma once

#include <cstddef>

class Shape;

class BaseActor
{
public:
    virtual ~BaseActor() = default;
    virtual size_t GetSize() = 0;
    virtual Shape* operator[](size_t) = 0;
};