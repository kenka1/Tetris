#pragma once

struct TimeData
{
    static double LastFrame;
    static double CurrentFrame;
    static double DeltaTime;

    static float time_step;
    static float time_move;
    static float time_move_line;
    static float time_space;
};

double TimeData::LastFrame = 0.0;
double TimeData::CurrentFrame = 0.0;
double TimeData::DeltaTime = 0.0;
float TimeData::time_step = 0.0f;
float TimeData::time_move = 0.0f;
float TimeData::time_move_line = 0.0f;
float TimeData::time_space = 0.0f;