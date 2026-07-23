#pragma once

#include <string_view>

namespace GameConfig
{
    constexpr std::string_view name = "Baba Is You";
    constexpr uint32_t windowWidth = 1280;
    constexpr uint32_t windowHeight = 720;

    constexpr uint32_t cellSize = 24;

    constexpr uint32_t gridWidth = windowWidth / cellSize;
    constexpr uint32_t gridHeight = windowHeight / cellSize;
}