#pragma once

// SDL2
#include <SDL2/SDL.h>

// std
#include <string_view>

namespace GameConfig
{
    constexpr std::string_view name = "Baba Is You";
    constexpr uint32_t windowWidth = 1280;
    constexpr uint32_t windowHeight = 720;
}