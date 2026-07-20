#include "renderer/fade.hpp"
#include "renderer/renderer.hpp"

#include "game/config.hpp"

#include "time/time.hpp"

// std
#include <algorithm>

Fade::Fade(Renderer& renderer)
    : m_renderer(renderer)
{
}

void Fade::update()
{
    if (m_state == FadeState::IDLE)
    {
        return;
    }

    m_duration = std::max(m_duration, s_minDuration);

    m_timer += Time::deltaTime();
    m_timer = std::min(m_timer, m_duration);

    switch (m_state)
    {
        case FadeState::OUT:
            m_col.a = (Uint8)((m_timer / m_duration) * 255);

            if (m_timer >= m_duration)
            {
                m_state = FadeState::IN;
                m_col.a = 255;
                m_timer = 0.0f;

                m_justFinishedOut = true;
            }

            break;

        case FadeState::IN:
            m_justFinishedOut = false;

            m_col.a = (Uint8)((1.0f - (m_timer / m_duration)) * 255);

            if (m_timer >= m_duration)
            {
                m_state = FadeState::IDLE;
                m_col.a = 0;
                m_timer = 0.0f;
            }
    }
}

void Fade::draw()
{
    SDL_Rect rect =
    {
        0,
        0,
        GameConfig::windowWidth,
        GameConfig::windowHeight
    };

    SDL_Renderer* renderer = m_renderer.getSDLRenderer();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, m_col.r, m_col.g, m_col.b, m_col.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Fade::start()
{
    m_state = FadeState::OUT;
}

void Fade::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    m_col.r = r;
    m_col.g = g;
    m_col.b = b;
}