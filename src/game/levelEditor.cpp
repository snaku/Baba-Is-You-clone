#include "game/levelEditor.hpp"
#include "game/level.hpp"
#include "game/grid.hpp"
#include "game/gridConfig.hpp"

#include "renderer/renderer.hpp"

#include "input/input.hpp"

LevelEditor::LevelEditor(Renderer& renderer, ObjectManager& objectMng, Grid& grid)
    : m_renderer(renderer),
      m_objectMng(objectMng),
      m_grid(grid)
{
}

bool LevelEditor::update(const Input& input)
{
    Cell newMouseCell;

    newMouseCell.fromFPoint(input.getMousePos());
    if (newMouseCell.isValidPos())
    {
        m_mouseCell = newMouseCell;

        handleInput(input);
    }

    return m_continueUpdate; // for the future when the editor will have a menu and a quit button
}

void LevelEditor::handleInput(const Input& input)
{
    if (input.isMouseButtonJustDown(SDL_BUTTON_LEFT))
    {
        m_objectMng.addObject(m_currObjectId, m_mouseCell);
    }
    else if (input.isMouseButtonJustDown(SDL_BUTTON_RIGHT))
    {
        auto objects = m_objectMng.findFromUIDs(m_grid.getObjectsAt(m_mouseCell));
        if (input.isKeyDown(SDL_SCANCODE_LSHIFT))
        {
            for (auto* object : objects)
            {
                m_objectMng.removeObject(*object);
            }
        }
        else
        {
            if (!objects.empty())
            {
                m_objectMng.removeObject(*objects.back());
            }
        }
    }

    int id = (int)m_currObjectId;
    if (input.scrolledUp())
    {
        id = (id + 1) % (int)ObjectId::MAX;
        if (id == (int)ObjectId::NONE)
        {
            id = (int)ObjectId::NONE + 1;
        }
    }
    else if (input.scrolledDown())
    {
        id = (id - 1) % (int)ObjectId::MAX;
        if (id == (int)ObjectId::NONE)
        {
            id = (int)ObjectId::MAX - 1;
        }
    }

    m_currObjectId = (ObjectId)id;
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