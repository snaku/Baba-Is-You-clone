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
    static std::chrono::high_resolution_clock::time_point m_lastTime;
    static std::chrono::high_resolution_clock::time_point m_startTime;

    static float m_deltaTime;
};