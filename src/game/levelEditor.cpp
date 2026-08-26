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

        if (input.isMouseButtonJustDown(SDL_BUTTON_LEFT))
        {
            m_objectMng.addObject(ObjectId::BABA, m_mouseCell); // test
        }
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

        (int)GridConfig::cellSize,
        (int)GridConfig::cellSize
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

LevelDefinition LevelEditor::createDef()
{
    LevelDefinition def{};

    def.width = GridConfig::width;
    def.height = GridConfig::height;

    m_objectMng.forEach(
        [&](const Object& object)
        {
            LevelObjectData data =
            {
                object.getId(),
                object.getCell()
            };

            def.objects.push_back(data);
        }
    );

    def.isValid = true;

    return def;
}