#include "game/grid.hpp"
#include "game/config.hpp"

// std
#include <algorithm>

Grid::Grid(uint32_t width, uint32_t height) 
    : m_objectsUID(height, std::vector<std::vector<std::size_t>>(width)),
      m_width(width),
      m_height(height)
{
}

void Grid::resize(uint32_t width, uint32_t height)
{
    m_objectsUID.assign(height, std::vector<std::vector<std::size_t>>(width));

    m_width = width;
    m_height = height;
}

void Grid::addObjectAt(std::size_t uid, Cell cell)
{
    if (!cell.isValidPos())
    {
        return;
    }

    m_objectsUID[cell.y][cell.x].push_back(uid);
}

void Grid::removeObjectAt(std::size_t uid, Cell cell)
{
    if (!cell.isValidPos())
    {
        return;
    }

    std::erase(m_objectsUID[cell.y][cell.x], uid);
}

void Grid::clearObjects()
{
    for (auto& row : m_objectsUID)
    {
        for (auto& cell : row)
        {
            cell.clear();
        }
    }
}

std::span<const std::size_t> Grid::getObjectsAt(Cell cell) const
{
    static std::vector<std::size_t> empty;

    if (!cell.isValidPos())
    {
        return empty;
    }

    return m_objectsUID[cell.y][cell.x];
}