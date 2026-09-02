#include "game/cell.hpp"
#include "game/gridConfig.hpp"

// std
#include <cmath>

bool Cell::isValidPos() const
{
    return x >= 0 && y >= 0 &&
           x < GridConfig::width &&
           y < GridConfig::height;
}

bool Cell::isInRect(const SDL_Rect& rect) const
{
    SDL_Rect cellRect = SDL_Rect
    {
        (int)toFPoint().x,
        (int)toFPoint().y,

        (int)GridConfig::cellSize,
        (int)GridConfig::cellSize
    };

    return SDL_HasIntersection(&cellRect, &rect);
}

void Cell::fromFPoint(const SDL_FPoint& point)
{
    x = (int)std::floor((point.x - GridConfig::offset.x) / GridConfig::cellSize);
    y = (int)std::floor((point.y - GridConfig::offset.y) / GridConfig::cellSize);
}

SDL_FPoint Cell::toFPoint() const
{
    SDL_FPoint point =
    {
        (float)(GridConfig::offset.x + x * GridConfig::cellSize),
        (float)(GridConfig::offset.y + y * GridConfig::cellSize)
    };

    return point;
}