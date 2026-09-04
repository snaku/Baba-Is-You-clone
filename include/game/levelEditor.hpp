#pragma once

#include "game/levelFile.hpp"
#include "game/cell.hpp"
#include "game/object.hpp"

#include "renderer/sprite.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <cstdint>
#include <vector>

class Renderer;
class Input;
class TextureManager;
class ObjectManager;
class Grid;

enum class LevelEditorAction
{
    NONE,

    ADD_OBJECT,
    REMOVE_OBJECT,
    CHANGE_OBJECT,
    MOVE_OBJECT,
    RESIZE_GRID,
    SELECT
};

class LevelEditor
{
public:
    LevelEditor(Renderer& renderer, TextureManager& textureMng, ObjectManager& objectMng, Grid& grid);

    bool update(const Input& input);
    void draw();

    LevelDefinition createDef();

private:
    void handleInput(const Input& input);

    void handleObjectPlacement(const Input& input);

    void handleObjectRemoval(const Input& input);
    bool tryRemoveSingleCellObject();
    bool tryRemoveSingleCellObjects();
    bool tryRemoveSelectedObjects();

    void handleObjectChange(const Input& input);
    ObjectId toNextObject();
    ObjectId toPrevObject();

    void handleObjectMove(const Input& input);
    bool tryMoveSelectedObjects();
    bool tryMoveSingleCellObject();
    bool tryMoveSingleCellObjects();
    void moveObject(Object& object, Cell cell);
    void moveObjectToMouse(Object& object);

    void handleGridResizing(const Input& input);

    void handleSelect(const Input& input);

    void changeObjectPreview(ObjectId id);

    void drawObjectPreview();
    void drawCellHighlight();
    void drawGrid();
    void drawMovingObjectsBaseCellHighlight();
    void drawSelectedObjectsCellHighlight();
    void drawSelectionRect();

    Renderer& m_renderer;
    ObjectManager& m_objectMng;
    Grid& m_grid;

    LevelEditorAction m_action = LevelEditorAction::NONE;

    Cell m_mouseCell;
    Cell m_prevMouseCell;
    bool m_mouseOnGrid = false;

    ObjectId m_currentObjectId = ObjectId::BABA;
    Sprite m_objectPreviewSpr;

    std::vector<Object*> m_movingObjects;
    Cell m_movingObjectsBaseCell; // for highlighting the original cell
    bool m_moving = false;

    SDL_Rect m_selectionRect{};
    SDL_Point m_selectionStart{};
    bool m_selecting = false;
    std::vector<Object*> m_selectedObjects;

    bool m_continueUpdate = true;

    static constexpr uint8_t s_objectPreviewAlpha = 128;
    static constexpr SDL_Color s_cellHighlightCol = SDL_Color{255, 255, 255, 100};
    static constexpr SDL_Color s_gridCol = SDL_Color{255, 255, 255, 128};
    static constexpr SDL_Color s_movingObjectsCellHighlightCol = SDL_Color{0, 128, 0, 160};
    static constexpr SDL_Color s_selectionRectCol = SDL_Color{0, 100, 160, 175};
    static constexpr SDL_Color s_selectedObjectsCellHighlightCol = SDL_Color{255, 255, 255, 130};
};