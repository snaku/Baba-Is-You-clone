#pragma once

#include <SDL2/SDL.h>

// std
#include <string_view>

namespace GameConfig
{
    inline constexpr std::string_view name = "Baba Is You";
    inline constexpr uint32_t windowWidth = 1280;
    inline constexpr uint32_t windowHeight = 720;

    inline uint32_t cellSize = 24;

    inline uint32_t gridWidth = windowWidth / cellSize;
    inline uint32_t gridHeight = windowHeight / cellSize;
    inline SDL_Point gridOffset {0, 0};
}