#pragma once
    
#include <cstdint>
#include "glm/glm.hpp"

class PlayerState
{
public:
    PlayerState();
    inline void SetID(int16_t new_id) {id = new_id;}
    inline void SetStop() {stop = true;}
    inline int16_t GetID() const {return id;}
    inline bool GetStop() const {return stop;}
    int16_t CalculateID(const glm::vec3&);
private:
    int16_t id;
    bool stop;
};