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

    static float MaxTime_step;
    static float MinTime_step;
    static float MaxTime_step_delta;
};

double TimeData::LastFrame = 0.0;
double TimeData::CurrentFrame = 0.0;
double TimeData::DeltaTime = 0.0;
float TimeData::time_step = 0.0f;
float TimeData::time_move = 0.0f;
float TimeData::time_move_line = 0.0f;
float TimeData::time_space = 0.0f;

float TimeData::MaxTime_step = 0.2f;
float TimeData::MinTime_step = 0.01;
float TimeData::MaxTime_step_delta = 0.00003;