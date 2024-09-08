#pragma once

struct TimeData
{
    static double LastFrame;
    static double CurrentFrame;

    static float time_step;
    static float time_move;
    static float time_move_line;
};

double TimeData::LastFrame = 0.0;
double TimeData::CurrentFrame = 0.0;
float TimeData::time_step = 0.0f;
float TimeData::time_move = 0.0f;
float time_move_line = 0.0f;