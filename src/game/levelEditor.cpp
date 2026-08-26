#include "game/levelEditor.hpp"
#include "game/level.hpp"
#include "game/gridConfig.hpp"

#include "renderer/renderer.hpp"

#include "input/input.hpp"

LevelEditor::LevelEditor(Renderer& renderer, ObjectManager& objectMng)
    : m_renderer(renderer),
      m_objectMng(objectMng)
{
}

bool LevelEditor::update(const Input& input)
{
    Cell newMouseCell;

    newMouseCell.fromFPoint(input.getMousePos());
    if (newMouseCell.isValidPos())
    {
        m_mouseCell = newMouseCell;
    }

    return m_continueUpdate;
}

void LevelEditor::draw()
{
    drawCellHighlight();
    drawGrid();
}

void LevelEditor::drawCellHighlight()
{
    SDL_Rect rect = SDL_Rect
    {
        (int)m_mouseCell.toFPoint().x,
        (int)m_mouseCell.toFPoint().y,

        GridConfig::cellSize,
        GridConfig::cellSize
    };

    m_renderer.drawRect(rect, {255, 255, 255, 128});
}

void LevelEditor::drawGrid()
{
    for (uint32_t i = 1; i < GridConfig::width; i++)
    {
        SDL_FPoint start
        {
            (float)i * GridConfig::cellSize + GridConfig::offset.x,
            (float)GridConfig::offset.y
        };

        m_renderer.drawLine(start,
                            (float)GridConfig::height * GridConfig::cellSize,
                            90.0f,
                            {255, 255, 255, 255});
    }

    for (uint32_t j = 1; j < GridConfig::height; j++)
    {
        SDL_FPoint start
        {
            (float)GridConfig::offset.x,
            (float)j * GridConfig::cellSize + GridConfig::offset.y
        };

        m_renderer.drawLine(start,
                            (float)GridConfig::width * GridConfig::cellSize,
                            0.0f,
                            {255, 255, 255, 255});
    }
}