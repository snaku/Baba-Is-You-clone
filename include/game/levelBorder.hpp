#pragma once

// SDL2
#include <SDL2/SDL.h>

// std
#include <array>
#include <utility>

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
    void drawSide(const SDL_Rect& side);

    Renderer& m_renderer;

    SDL_Rect m_levelArea{};
    std::array<SDL_Rect, std::to_underlying(LevelBorderSide::MAX)> m_sides;

    static constexpr SDL_Color s_color {60, 60, 60, 255};
};