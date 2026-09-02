#pragma once

// SDL2
#include <SDL2/SDL.h>

struct Cell
{
    int x = 0;
    int y = 0;

    bool isValidPos() const;
    bool isInRect(const SDL_Rect& rect) const;

    void fromFPoint(const SDL_FPoint& point);
    SDL_FPoint toFPoint() const;

    bool operator==(Cell other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(Cell other) const
    {
        return x != other.x || y != other.y;
    }
};