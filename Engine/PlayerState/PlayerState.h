#pragma once
    
#include <cstdint>
#include <glm/glm.hpp>

#include <vector>

class GameMode;

class PlayerState
{
public:
    PlayerState(GameMode*);

    void SetID(int16_t, size_t);
    inline void SetStop(bool flag) {stop = flag;}

    inline int16_t GetID(size_t index) const {return id[index];}
    inline bool GetStop() const {return stop;}

    int16_t CalculateID(const glm::vec3&);
private:
    GameMode* game;
    std::vector<int16_t> id;
    bool stop;
};