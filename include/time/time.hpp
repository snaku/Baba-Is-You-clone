#pragma once

// std
#include <chrono>

class Time
{
public:
    static void init();
    static void update();

    static float deltaTime();
    static float totalTime();

private:
    inline static std::chrono::high_resolution_clock::time_point m_lastTime;
    inline static std::chrono::high_resolution_clock::time_point m_startTime;

    inline static float m_deltaTime = 0.0f;
};