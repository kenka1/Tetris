#pragma once
    
#include <cstdint>
#include <glm/glm.hpp>

class GameMode;
class Actor;

class PlayerState
{
public:
    PlayerState(GameMode*);

    inline void SetID(int16_t new_id) {id = new_id;}
    inline void SetStop(bool flag) {stop = flag;}

    inline int16_t GetID() const {return id;}
    inline bool GetStop() const {return stop;}

    int16_t CalculateID(const glm::vec3&);
private:
    GameMode* game;
    int16_t id;
    bool stop;
};