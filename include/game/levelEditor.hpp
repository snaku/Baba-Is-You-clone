#pragma once

#include "game/levelFile.hpp"
#include "game/cell.hpp"
#include "game/object.hpp"

#include "renderer/sprite.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <cstdint>

class Renderer;
class Input;
class TextureManager;
class ObjectManager;
class Grid;

class LevelEditor
{
public:
    LevelEditor(Renderer& renderer, TextureManager& textureMng, ObjectManager& objectMng, Grid& grid);

    bool update(const Input& input);
    void draw();

    LevelDefinition createDef();

private:
    void handleInput(const Input& input);

    void drawObjectPreview();
    void drawCellHighlight();
    void drawGrid();

    Renderer& m_renderer;
    ObjectManager& m_objectMng;
    Grid& m_grid;

    Cell m_mouseCell;

    ObjectId m_currentObjectId = ObjectId::BABA;
    Sprite m_objectPreviewSpr;

    bool m_continueUpdate = true;

    static constexpr uint8_t s_objectPreviewAlpha = 128;
    static constexpr SDL_Color s_cellHighlightCol = SDL_Color{255, 255, 255, 100};
};