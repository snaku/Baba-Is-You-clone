#pragma once

#include "game/object.hpp"

// std
#include <vector>

class ObjectManager;
class Grid;

namespace GameUtils
{

Cell getNextCellFromDir(Cell cell, Direction dir);
void getObjectsAt(ObjectManager& objectMng, const Grid& grid, Cell cell, std::vector<Object*>& out);

}