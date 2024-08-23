#pragma once
    
#include <cstdint>
#include <glm/glm.hpp>

#include <vector>

class GameMode;

class PlayerState
{
public:
    PlayerState(GameMode*);

    inline void SetID(size_t index, int16_t new_id) {Grid_ID[index] = new_id;}
    inline int16_t GetID(size_t index) const {return Grid_ID[index];}

    inline void SetStop(bool flag) {stop = flag;}
    inline bool GetStop() const {return stop;}

    int16_t CalculateID(const glm::vec3&);
    static int16_t Player_ID;

private:
    GameMode* game;
    std::vector<int16_t> Grid_ID;
    bool stop;
};