#include "game/utils.hpp"
#include "game/grid.hpp"
#include "game/objectManager.hpp"

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

// TODO: move this function
void getObjectsAt(ObjectManager& objectMng,
                  const Grid& grid,
                  Cell cell,
                  std::vector<Object*>& out)
{
    const std::vector<std::size_t>& objectsUID = grid.getObjectsAt(cell);

    out.clear();
    out.reserve(objectsUID.size());

    for (auto uid : objectsUID)
    {
        Object* object = objectMng.findObjectFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        out.push_back(object);
    }
}

}