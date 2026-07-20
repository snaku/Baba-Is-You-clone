#pragma once

#include <string_view>

namespace GameConfig
{
    constexpr std::string_view name = "Baba Is You";
    constexpr int windowWidth = 1280;
    constexpr int windowHeight = 720;

    constexpr int cellSize = 24;

    constexpr int gridWidth = windowWidth / cellSize;
    constexpr int gridHeight = windowHeight / cellSize;
}