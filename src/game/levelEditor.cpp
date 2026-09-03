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
        m_prevMouseCell = m_mouseCell;
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

    handleSelect(input);
    handleObjectMove(input);
    handleObjectPlacement(input);
    handleObjectRemoval(input);
    handleGridResizing(input);
    handleObjectChange(input);
}

void LevelEditor::handleObjectPlacement(const Input& input)
{
    if (m_action == LevelEditorAction::MOVE_OBJECT ||
        m_action == LevelEditorAction::SELECT ||
        !input.isMouseButtonJustDown(SDL_BUTTON_LEFT))
    {
        return;
    }

    m_objectMng.addObject(m_currentObjectId, m_mouseCell);

    m_selectedObjects.clear();

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
        // remove every objects at the cell
        for (auto* object : objects)
        {
            m_objectMng.removeObject(*object);
        }
    }
    else
    {
        // remove the last one placed
        m_objectMng.removeObject(*objects.back());
    }

    m_selectedObjects.clear();

    m_action = LevelEditorAction::REMOVE_OBJECT;
}

void LevelEditor::handleObjectChange(const Input& input)
{
    if (m_action == LevelEditorAction::RESIZE_GRID)
    {
        return;
    }

    ObjectId savedId = m_currentObjectId;
    if (input.scrolledUp())
    {
        m_currentObjectId = toNextObject();
    }
    else if (input.scrolledDown())
    {
        m_currentObjectId = toPrevObject();
    }

    if (savedId != m_currentObjectId)
    {
        changeObjectPreview(m_currentObjectId);
        m_action = LevelEditorAction::CHANGE_OBJECT;
    }
}

ObjectId LevelEditor::toNextObject()
{
    int id = (int)m_currentObjectId;

    id = (id + 1) % (int)ObjectId::MAX;
    if (id == (int)ObjectId::NONE)
    {
        id = (int)ObjectId::NONE + 1;
    }

    return (ObjectId)id;
}

ObjectId LevelEditor::toPrevObject()
{
    int id = (int)m_currentObjectId;

    id = (id - 1) % (int)ObjectId::MAX; // it's ok since m_currentObjectId will never be ObjectId::NONE
    if (id == (int)ObjectId::NONE)
    {
        id = (int)ObjectId::MAX - 1;
    }

    return (ObjectId)id;
}

void LevelEditor::handleObjectMove(const Input& input)
{
    if (m_action == LevelEditorAction::SELECT ||
        !input.isKeyDown(SDL_SCANCODE_LCTRL) ||
        !input.isMouseButtonDown(SDL_BUTTON_LEFT))
    {
        m_movingObjects.clear();
        m_movingObjectsBaseCell = {};
        m_moving = false;
        return;
    }

    if ((input.isKeyDown(SDL_SCANCODE_LSHIFT) && tryMoveSingleCellObjects()) ||
        tryMoveSelectedObjects() ||
        tryMoveSingleCellObject())
    {
        m_moving = true;
        m_action = LevelEditorAction::MOVE_OBJECT;
    }
    else
    {
        m_moving = false;
    }
}

bool LevelEditor::tryMoveSelectedObjects()
{
    if (m_selectedObjects.empty())
    {
        return false;
    }

    if (m_movingObjects.empty())
    {
        m_movingObjects = m_selectedObjects;
        m_movingObjectsBaseCell = m_mouseCell;

        bool foundObjectInMouseCell = 
        std::any_of(m_movingObjects.begin(),
                    m_movingObjects.end(),
                    [this](Object* object)
                    {
                        return object->getCell() == m_mouseCell;
                    }
        );

        if (!foundObjectInMouseCell)
        {
            m_movingObjects.clear();
            m_movingObjectsBaseCell = {};
            return false;
        }
    }

    Cell delta = m_mouseCell - m_prevMouseCell;
    bool canMove = true;
    for (auto* object : m_movingObjects)
    {
        Cell finalCell = object->getCell() + delta;
        if (!finalCell.isValidPos())
        {
            canMove = false;
            break;
        }
    }

    if (canMove)
    {
        for (auto* object : m_movingObjects)
        {
            moveObject(*object, object->getCell() + delta);
        }
    }

    return true;
}

bool LevelEditor::tryMoveSingleCellObjects()
{
    if (m_movingObjects.empty())
    {
        m_movingObjects = m_objectMng.findFromUIDs(m_grid.getObjectsAt(m_mouseCell));
        if (m_movingObjects.empty())
        {
            return false;
        }

        m_movingObjectsBaseCell = m_mouseCell;
    }

    // move every obejcts at the cell 
    for (auto* object : m_movingObjects)
    {
        if (object->getCell() == m_mouseCell)
        {
            break;
        }

        moveObjectToMouse(*object);
    }

    return true;
}

bool LevelEditor::tryMoveSingleCellObject()
{
    if (m_movingObjects.empty())
    {
        m_movingObjects = m_objectMng.findFromUIDs(m_grid.getObjectsAt(m_mouseCell));
        if (m_movingObjects.empty())
        {
            return false;
        }

        m_movingObjectsBaseCell = m_mouseCell;
    }

    // move the last one placed
    moveObjectToMouse(*m_movingObjects.back());

    return true;
}

void LevelEditor::moveObject(Object& object, Cell cell)
{
    m_grid.removeObjectAt(object.getUID(), object.getCell());
    object.setCell(cell);
    m_grid.addObjectAt(object.getUID(), object.getCell());
}

void LevelEditor::moveObjectToMouse(Object& object)
{
    if (object.getCell() == m_mouseCell)
    {
        return;
    }

    moveObject(object, m_mouseCell);
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

        m_selectedObjects.clear();

        m_action = LevelEditorAction::RESIZE_GRID;
    }
    else if (input.scrolledDown())
    {
        GridConfig::width = std::max(GridConfig::width - 1, (uint32_t)1);
        GridConfig::height = std::max(GridConfig::height - 1, (uint32_t)1);

        // if some objects are outside the new grid
        m_objectMng.removeIf(
            [](const Object& object)
            {
                return !object.getCell().isValidPos();
            }
        );

        m_selectedObjects.clear();

        m_action = LevelEditorAction::RESIZE_GRID;
    }
}

void LevelEditor::handleSelect(const Input& input)
{
    if (!input.isKeyDown(SDL_SCANCODE_LALT) ||
        !input.isMouseButtonDown(SDL_BUTTON_LEFT))
    {
        m_selecting = false;
        return;
    }

    SDL_Point mousePos = SDL_Point
    {
        (int)input.getMousePos().x,
        (int)input.getMousePos().y
    };

    if (!m_selecting)
    {
        m_selectionStart = SDL_Point
        {
            mousePos.x,
            mousePos.y
        };
    }

    m_selectionRect.x = std::min(m_selectionStart.x, mousePos.x);
    m_selectionRect.y = std::min(m_selectionStart.y, mousePos.y);
    m_selectionRect.w = std::max(m_selectionStart.x, mousePos.x) - m_selectionRect.x;
    m_selectionRect.h = std::max(m_selectionStart.y, mousePos.y) - m_selectionRect.y;

    m_selectedObjects.clear();
    m_objectMng.forEach(
        [this](Object& object)
        {
            if (object.getCell().isInRect(m_selectionRect))
            {
                m_selectedObjects.push_back(&object);
            }
        }
    );

    m_selecting = true;
    m_action = LevelEditorAction::SELECT;
}

void LevelEditor::changeObjectPreview(ObjectId id)
{
    SpriteInfo previewSprInfo = ObjectUtils::getSpriteInfo(id);
    previewSprInfo.col.a = s_objectPreviewAlpha;
    m_objectPreviewSpr.reload(previewSprInfo);
}

void LevelEditor::draw()
{
    drawGrid();

    if (m_mouseOnGrid)
    {
        if (m_selecting)
        {
            drawSelectionRect();
        }
        else if (m_moving)
        {
            drawMovingObjectsBaseCellHighlight();
        }
        else
        {
            drawObjectPreview();
            drawCellHighlight();
        }
    }

    if (!m_selectedObjects.empty())
    {
        drawSelectedObjectsCellHighlight();
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

void LevelEditor::drawSelectedObjectsCellHighlight()
{
    for (auto* object : m_selectedObjects)
    {
        SDL_FPoint pos = object->getCell().toFPoint();

        SDL_Rect rect = SDL_Rect
        {
            (int)pos.x,
            (int)pos.y,
    
            (int)GridConfig::cellSize,
            (int)GridConfig::cellSize
        };
    
        m_renderer.drawRect(rect, s_selectedObjectsCellHighlightCol);
    }
}

void LevelEditor::drawSelectionRect()
{
    m_renderer.drawRect(m_selectionRect, s_selectionRectCol);
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