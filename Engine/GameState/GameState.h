#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

class Shape;

struct PlayerInfo
{
    Shape* Target = nullptr;
    int16_t Player_ID = -1;
    bool stop = false;
};

class GameState
{
public:
    GameState();
    ~GameState();

    void AddToGrid(Shape*, int16_t, int16_t);
    inline void ClearGrid(int16_t index) {
        Grid[index].Target = nullptr;
        Grid[index].Player_ID = -1;
        Grid[index].stop = false;
    }
    bool CheckCell(int16_t, int16_t);
    void RemoveLine(int16_t);
    void MoveLine(int16_t);
    inline std::vector<PlayerInfo>& GetGrid() {return Grid;}
private:
    std::vector<PlayerInfo> Grid;
    size_t size;
};