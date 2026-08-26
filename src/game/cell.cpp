#include "game/cell.hpp"
#include "game/gridConfig.hpp"

bool Cell::isValidPos() const
{
    return x >= 0 && y >= 0 &&
           x < GridConfig::width &&
           y < GridConfig::height;
}

void Cell::fromFPoint(const SDL_FPoint& point)
{
    x = (int)((point.x - GridConfig::offset.x) / GridConfig::cellSize);
    y = (int)((point.y - GridConfig::offset.y) / GridConfig::cellSize);
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