#include "game/levelTransition.hpp"
#include "game/level.hpp"

#include "renderer/fade.hpp"

LevelTransition::LevelTransition(Level& level, Fade& fade)
    : m_level(level),
      m_fade(fade)
{
}

void LevelTransition::update()
{
    if (m_level.reloadRequested() &&
        !m_fade.isActive())
    {
        m_fade.start();
    }
    else if (m_fade.isActive() &&
             m_fade.justFinishedOut())
    {
        m_level.allowReload();
    }
}

void LevelTransition::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    m_fade.setColor(r, g, b);
}