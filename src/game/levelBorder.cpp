#include "game/levelBorder.hpp"
#include "game/config.hpp"
#include "game/gridConfig.hpp"

#include "renderer/renderer.hpp"

LevelBorder::LevelBorder(Renderer& renderer)
    : m_renderer(renderer)
{
}

void LevelBorder::resize(uint32_t windowWidth,
                         uint32_t windowHeight,
                         uint32_t gridWidth,
                         uint32_t gridHeight)
{
    m_levelArea = 
    {
        GridConfig::offset.x,
        GridConfig::offset.y,

        (int)(gridWidth * GridConfig::cellSize),
        (int)(gridHeight * GridConfig::cellSize)
    };

    m_sides[std::to_underlying(LevelBorderSide::TOP)] =
    {
        0,
        0,

        (int)windowWidth,
        m_levelArea.y
    };

    m_sides[std::to_underlying(LevelBorderSide::BOT)] =
    {
        0,
        m_levelArea.y + m_levelArea.h,

        (int)windowWidth,
        (int)(windowHeight - (m_levelArea.y + m_levelArea.h))
    };

    m_sides[std::to_underlying(LevelBorderSide::LEFT)] = 
    {
        0,
        m_levelArea.y,

        m_levelArea.x,
        m_levelArea.h
    };

    m_sides[std::to_underlying(LevelBorderSide::RIGHT)] =
    {
        m_levelArea.x + m_levelArea.w,
        m_levelArea.y,

        (int)(windowWidth - (m_levelArea.x + m_levelArea.w)),
        m_levelArea.h
    };
}

void LevelBorder::draw()
{
    for (const auto& side : m_sides)
    {
        drawSide(side);
    }
}

void LevelBorder::drawSide(const SDL_Rect& side)
{
    if (side.w <= 0 ||
        side.h <= 0)
    {
        return;
    }

    m_renderer.drawRect(side, s_color);
}