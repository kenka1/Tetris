#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include "glm/glm.hpp"
#include "Type/Types.h"

class Shape;

struct PlayerInfo
{
    Shape* Target = nullptr;
    int16_t Player_ID = -1;
    bool stop = false;
    EForm type;
};

class GameState
{
public:
    GameState();
    ~GameState();

    void AddToGrid(Shape*, int16_t, int16_t, EForm);
    void UpdatePredictGrid(const std::vector<glm::vec3>&);
    inline void ClearGrid(int16_t index) {
        Grid[index].Target = nullptr;
        Grid[index].Player_ID = -1;
        Grid[index].stop = false;
    }
    bool CheckCell(int16_t, int16_t);
    void RemoveLine(int16_t);
    void MoveLine();
    inline std::vector<PlayerInfo>& GetGrid() {return Grid;}
    inline std::vector<Shape*>& GetPredictGrid() {return PredictGrid;}
    inline bool& GetDelay() {return delay;}
    inline int GetDelay() const {return remove_count;}
    void ResetDraw();
private:
    std::vector<PlayerInfo> Grid;
    std::vector<Shape*> PredictGrid;
    size_t size;
    uint16_t score = 0;
    bool delay = false;
    int remove_count = 0;
    int16_t _line_index = -1;

public:
    std::vector<EForm> Draw;
    std::vector<EForm> Queue;
};