#pragma once

#include "game/cell.hpp"

// std
#include <vector>

class Level;

class Grid
{
public:
    Grid(Level& level, int width, int height);

    void addObjectAt(std::size_t uid, Cell cell);
    void removeObjectAt(std::size_t uid, Cell cell);
    void clearObjects();
    const std::vector<std::size_t>& getObjectsAt(Cell cell) const;

private:
    Level& m_level;
    std::vector<std::vector<std::vector<std::size_t>>> m_objectsUID;
    int m_width;
    int m_height;
};