#include "time/time.hpp"

std::chrono::high_resolution_clock::time_point Time::m_lastTime;
std::chrono::high_resolution_clock::time_point Time::m_startTime;

float Time::m_deltaTime = 0.0f;

void Time::init()
{
    m_startTime = std::chrono::high_resolution_clock::now();
    m_lastTime = m_startTime;
}

void Time::update()
{
    auto currentTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> elapsed = currentTime - m_lastTime;

    m_deltaTime = elapsed.count();

    m_lastTime = currentTime;
}

float Time::deltaTime()
{
    return m_deltaTime;
}

float Time::totalTime()
{
    std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - m_startTime;

    return elapsed.count();
}