#pragma once

// SDL2
#include <SDL2/SDL.h>

class Level;
class Fade;

class LevelTransition
{
public:
    LevelTransition(Level& level, Fade& fade);

    void update();

    void setColor(Uint8 r, Uint8 g, Uint8 b);

private:
    Level& m_level;
    Fade& m_fade;
};