#include "game/cell.hpp"
#include "game/config.hpp"

bool Cell::isValidPos() const
{
    return x >= 0 && y >= 0 &&
           x < GameConfig::gridWidth &&
           y < GameConfig::gridHeight;
}

SDL_FPoint Cell::toFPoint() const
{
    SDL_FPoint point =
    {
        (float)(x * GameConfig::cellSize),
        (float)(y * GameConfig::cellSize)
    };

    return point;
}