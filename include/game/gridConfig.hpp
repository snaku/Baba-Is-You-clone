#pragma once

#include "game/config.hpp"

namespace GridConfig
{
    inline uint32_t cellSize = 24;

    inline uint32_t width = GameConfig::windowWidth / cellSize;
    inline uint32_t height = GameConfig::windowHeight / cellSize;
    inline SDL_Point offset {0, 0};
}