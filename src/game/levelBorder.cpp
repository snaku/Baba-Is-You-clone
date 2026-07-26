#include "game/levelBorder.hpp"
#include "game/config.hpp"

#include "renderer/renderer.hpp"

LevelBorder::LevelBorder(Renderer& renderer)
    : m_renderer(renderer)
{
}

void LevelBorder::resize(uint32_t gridWidth, uint32_t gridHeight)
{
    m_levelArea = 
    {
        GameConfig::gridOffset.x,
        GameConfig::gridOffset.y,

        (int)(gridWidth * GameConfig::cellSize),
        (int)(gridHeight * GameConfig::cellSize)
    };

    m_borders[(std::size_t)LevelBorderSide::TOP] =
    {
        0,
        0,

        (int)GameConfig::windowWidth,
        m_levelArea.y
    };

    m_borders[(std::size_t)LevelBorderSide::BOT] =
    {
        0,
        m_levelArea.y + m_levelArea.h,

        (int)GameConfig::windowWidth,
        (int)(GameConfig::windowHeight - (m_levelArea.y + m_levelArea.h))
    };

    m_borders[(std::size_t)LevelBorderSide::LEFT] = 
    {
        0,
        m_levelArea.y,

        m_levelArea.x,
        m_levelArea.h
    };

    m_borders[(std::size_t)LevelBorderSide::RIGHT] =
    {
        m_levelArea.x + m_levelArea.w,
        m_levelArea.y,

        (int)(GameConfig::windowWidth - (m_levelArea.x + m_levelArea.w)),
        m_levelArea.h
    };
}

void LevelBorder::draw()
{
    for (const auto& rect : m_borders)
    {
        drawIfVisible(rect);
    }
}

void LevelBorder::drawIfVisible(const SDL_Rect& rect)
{
    if (rect.w <= 0 ||
        rect.h <= 0)
    {
        return;
    }

    m_renderer.drawRect(rect, s_color);
}