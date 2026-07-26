#pragma once

// SDL2
#include <SDL2/SDL.h>

// std
#include <array>

class Renderer;

enum class LevelBorderSide
{
    TOP,
    BOT,
    LEFT,
    RIGHT,

    MAX
};

class LevelBorder
{
public:
    explicit LevelBorder(Renderer& renderer);

    void resize(uint32_t gridWidth, uint32_t gridHeight);
    void draw();

private:
    void drawIfVisible(const SDL_Rect& rect);

    Renderer& m_renderer;

    SDL_Rect m_levelArea{};
    std::array<SDL_Rect, (std::size_t)LevelBorderSide::MAX> m_borders;

    static constexpr SDL_Color s_color {60, 60, 60, 255};
};