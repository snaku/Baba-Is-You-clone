#include "time/time.hpp"

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