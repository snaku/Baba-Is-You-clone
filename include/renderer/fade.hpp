#pragma once

// SDL2
#include <SDL2/SDL.h>

class Renderer;

enum class FadeState
{
    IDLE,
    OUT,
    IN,
};

class Fade
{
public:
    explicit Fade(Renderer& renderer);

    void update();
    void draw();

    void start();

    bool isActive() const { return m_state != FadeState::IDLE; }
    bool justFinishedOut() const { return m_justFinishedOut; }

    void setDuration(float duration) { m_duration = duration; }
    void setColor(Uint8 r, Uint8 g, Uint8 b);

private:
    Renderer& m_renderer;

    FadeState m_state = FadeState::IDLE;
    float m_duration = 0.5f;
    float m_timer = 0.0f;
    SDL_Color m_col {0, 0, 0, 0};

    bool m_justFinishedOut = false;
};