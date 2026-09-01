#include "game/levelEditor.hpp"
#include "game/level.hpp"
#include "game/objectUtils.hpp"
#include "game/grid.hpp"
#include "game/gridConfig.hpp"

#include "renderer/renderer.hpp"

#include "input/input.hpp"

LevelEditor::LevelEditor(Renderer& renderer,
                         TextureManager& textureMng,
                         ObjectManager& objectMng,
                         Grid& grid)
    : m_renderer(renderer),
      m_objectMng(objectMng),
      m_grid(grid),
      m_objectPreviewSpr(renderer,
                         textureMng,
                         ObjectUtils::getSpriteInfo(m_currentObjectId),
                         SDL_FPoint{0.0f, 0.0f})
{
    SDL_Color previewCol = m_objectPreviewSpr.getColor();
    previewCol.a = s_objectPreviewAlpha;
    m_objectPreviewSpr.setColor(previewCol);
}

bool LevelEditor::update(const Input& input)
{
    Cell newMouseCell;

    newMouseCell.fromFPoint(input.getMousePos());
    m_mouseOnGrid = newMouseCell.isValidPos();
    if (m_mouseOnGrid)
    {
        m_mouseCell = newMouseCell;

        m_objectPreviewSpr.setPos(m_mouseCell.toFPoint());

        handleInput(input);
    }

    m_objectPreviewSpr.updateAnim();

    return m_continueUpdate; // for the future when the editor will have a menu and a quit button
}

void LevelEditor::handleInput(const Input& input)
{
    m_action = LevelEditorAction::NONE;

    handleObjectMove(input);
    handleObjectPlacement(input);
    handleObjectRemoval(input);
    handleGridResizing(input);
    handleObjectSelection(input);
}

void LevelEditor::handleObjectPlacement(const Input& input)
{
    if (m_action == LevelEditorAction::MOVE_OBJECT ||
        !input.isMouseButtonJustDown(SDL_BUTTON_LEFT))
    {
        return;
    }

    m_objectMng.addObject(m_currentObjectId, m_mouseCell);
    m_action = LevelEditorAction::ADD_OBJECT;
}

void LevelEditor::handleObjectRemoval(const Input& input)
{
    if (m_action != LevelEditorAction::NONE ||
        !input.isMouseButtonJustDown(SDL_BUTTON_RIGHT))
    {
        return;
    }

    auto objects = m_objectMng.findFromUIDs(m_grid.getObjectsAt(m_mouseCell));
    if (objects.empty())
    {
        return;
    }

    if (input.isKeyDown(SDL_SCANCODE_LSHIFT))
    {
        for (auto* object : objects)
        {
            m_objectMng.removeObject(*object);
        }
    }
    else
    {
        m_objectMng.removeObject(*objects.back());
    }

    m_action = LevelEditorAction::REMOVE_OBJECT;
}

void LevelEditor::handleObjectSelection(const Input& input)
{
    if (m_action == LevelEditorAction::RESIZE_GRID)
    {
        return;
    }

    int id = (int)m_currentObjectId;
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

    if (id != (int)m_currentObjectId)
    {
        changeObjectPreview((ObjectId)id);
        m_action = LevelEditorAction::SELECT_OBJECT;
    }
}

void LevelEditor::handleObjectMove(const Input& input)
{
    if (!input.isKeyDown(SDL_SCANCODE_LCTRL) ||
        !input.isMouseButtonDown(SDL_BUTTON_LEFT))
    {
        m_movingObjects.clear();
        m_movingObjectsBaseCell = {};
        return;
    }

    if (m_movingObjects.empty())
    {
        m_movingObjects = m_objectMng.findFromUIDs(m_grid.getObjectsAt(m_mouseCell));
        if (m_movingObjects.empty())
        {
            return;
        }

        m_movingObjectsBaseCell = m_mouseCell;
    }

    if (input.isKeyDown(SDL_SCANCODE_LSHIFT))
    {
        for (auto* object : m_movingObjects)
        {
            if (object->getCell() == m_mouseCell)
            {
                break;
            }

            m_grid.removeObjectAt(object->getUID(), object->getCell());
            object->setCell(m_mouseCell);
            m_grid.addObjectAt(object->getUID(), object->getCell());
        }
    }
    else
    {
        Object& object = *m_movingObjects.back();

        if (object.getCell() != m_mouseCell)
        {
            m_grid.removeObjectAt(object.getUID(), object.getCell());
            object.setCell(m_mouseCell);
            m_grid.addObjectAt(object.getUID(), object.getCell());
        }
    }

    m_action = LevelEditorAction::MOVE_OBJECT;
}

void LevelEditor::handleGridResizing(const Input& input)
{
    if (m_action == LevelEditorAction::MOVE_OBJECT ||
        !input.isKeyDown(SDL_SCANCODE_LCTRL))
    {
        return;
    }

    if (input.scrolledUp())
    {
        GridConfig::width++;
        GridConfig::height++;

        m_action = LevelEditorAction::RESIZE_GRID;
    }
    else if (input.scrolledDown())
    {
        GridConfig::width = std::max(GridConfig::width - 1, (uint32_t)1);
        GridConfig::height = std::max(GridConfig::height - 1, (uint32_t)1);

        m_objectMng.removeIf(
            [](const Object& object)
            {
                return !object.getCell().isValidPos();
            }
        );

        m_action = LevelEditorAction::RESIZE_GRID;
    }
}

void LevelEditor::changeObjectPreview(ObjectId id)
{
    SpriteInfo previewSprInfo = ObjectUtils::getSpriteInfo(id);
    previewSprInfo.col.a = s_objectPreviewAlpha;
    m_objectPreviewSpr.reload(previewSprInfo);

    m_currentObjectId = id;
}

void LevelEditor::draw()
{
    drawGrid();

    if (m_mouseOnGrid)
    {
        drawObjectPreview();
        drawCellHighlight();
    }

    if (!m_movingObjects.empty())
    {
        drawMovingObjectsBaseCellHighlight();
    }
}

void LevelEditor::drawObjectPreview()
{
    m_objectPreviewSpr.draw(GridConfig::cellSize, GridConfig::cellSize);
}

void LevelEditor::drawCellHighlight()
{
    SDL_FPoint mousePos = m_mouseCell.toFPoint();

    SDL_Rect rect = SDL_Rect
    {
        (int)mousePos.x,
        (int)mousePos.y,

        (int)GridConfig::cellSize,
        (int)GridConfig::cellSize
    };

    m_renderer.drawRect(rect, s_cellHighlightCol);
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
                            s_gridCol);
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
                            s_gridCol);
    }
}

void LevelEditor::drawMovingObjectsBaseCellHighlight()
{
    SDL_FPoint pos = m_movingObjectsBaseCell.toFPoint();

    SDL_Rect rect = SDL_Rect
    {
        (int)pos.x,
        (int)pos.y,

        (int)GridConfig::cellSize,
        (int)GridConfig::cellSize
    };

    m_renderer.drawRect(rect, s_movingObjectsCellHighlightCol);
}

LevelDefinition LevelEditor::createDef()
{
    LevelDefinition def{};

    def.width = GridConfig::width;
    def.height = GridConfig::height;

    m_objectMng.forEach(
        [&](const Object& object)
        {
            def.objects.push_back({object.getId(), object.getCell()});
        }
    );

    def.isValid = true;

    return def;
}