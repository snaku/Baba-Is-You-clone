#pragma once

#include "game/cell.hpp"

// std
#include <vector>

class Level;

class Grid
{
public:
    Grid(uint32_t width, uint32_t height);

    void addObjectAt(std::size_t uid, Cell cell);
    void removeObjectAt(std::size_t uid, Cell cell);
    void clearObjects();
    const std::vector<std::size_t>& getObjectsAt(Cell cell) const;

private:
    std::vector<std::vector<std::vector<std::size_t>>> m_objectsUID;
    uint32_t m_width;
    uint32_t m_height;
};