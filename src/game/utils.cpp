#include "game/utils.hpp"

namespace GameUtils
{

Cell getNextCellFromDir(Cell cell, Direction dir)
{
    Cell next = cell;

    switch (dir)
    {
        case Direction::UP:
            next.y--;
            break;
        case Direction::DOWN:
            next.y++;
            break;
        case Direction::LEFT:
            next.x--;
            break;
        case Direction::RIGHT:
            next.x++;
            break;

        default: return cell;
    }

    return next;
}

}