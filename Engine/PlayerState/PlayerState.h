#pragma once

#include <cstdint>
#include "glm/glm.hpp"

class PlayerState
{
public:
    PlayerState();
    inline void SetID(int16_t new_id) {id = new_id;}
    int16_t CalculateID(const glm::vec3&);
private:
    int16_t id;

};