#pragma once

class Level;
class Fade;

class LevelTransition
{
public:
    LevelTransition(Level& level, Fade& fade);

    void update();

private:
    Level& m_level;
    Fade& m_fade;
};