#pragma once

#include <SDL2/SDL.h>

struct Cell
{
    int x;
    int y;

    bool isValidPos() const;
    SDL_FPoint toFPoint() const;

    bool operator==(Cell other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(Cell other) const
    {
        return x != other.x && y != other.y;
    }
};