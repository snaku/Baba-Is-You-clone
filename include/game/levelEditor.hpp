#pragma once

#include "game/levelFile.hpp"
#include "game/cell.hpp"

class Renderer;
class Input;
class ObjectManager;

class LevelEditor
{
public:
    LevelEditor(Renderer& renderer, ObjectManager& objectMng);

    bool update(const Input& input);
    void draw();

    LevelDefinition createDef();

private:
    void drawCellHighlight();
    void drawGrid();

    Renderer& m_renderer;
    ObjectManager& m_objectMng;

    Cell m_mouseCell;
    bool m_continueUpdate = true;
};