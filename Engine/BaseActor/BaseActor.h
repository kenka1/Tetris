#pragma once

#include <cstddef>
#include <cstdint>

class Shape;

class BaseActor
{
public:
    virtual ~BaseActor() = default;
    virtual size_t GetSize() = 0;
    virtual Shape* operator[](size_t) = 0;
    virtual int16_t GetID() const = 0;
};