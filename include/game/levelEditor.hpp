#pragma once

#include "game/levelFile.hpp"
#include "game/cell.hpp"
#include "game/object.hpp"

class Renderer;
class Input;
class ObjectManager;
class Grid;

class LevelEditor
{
public:
    LevelEditor(Renderer& renderer, ObjectManager& objectMng, Grid& grid);

    bool update(const Input& input);
    void draw();

    LevelDefinition createDef();

private:
    void handleInput(const Input& input);

    void drawCellHighlight();
    void drawGrid();

    Renderer& m_renderer;
    ObjectManager& m_objectMng;
    Grid& m_grid;

    Cell m_mouseCell;
    ObjectId m_currObjectId = ObjectId::BABA;
    bool m_continueUpdate = true;
};