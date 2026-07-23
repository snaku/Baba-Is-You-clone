#pragma once

#include "game/object.hpp"

class Input;
class Grid;
class ObjectManager;
class RuleSystem;

class MovementSystem
{
public:
    MovementSystem(ObjectManager& objectMng, Grid& grid, RuleSystem& ruleSystem, const Input& input);

    bool tryMoveYou(Object& object);
    bool updateMoveTimer();
private:
    bool handleSinkInteraction(Object& object, Object& other);
    bool handlePushInteraction(Object& object, Direction dir);
    bool handleObjectInteractionsAt(Object& object, Cell cell, Direction dir);

    bool tryMove(Object& object, Direction dir);

    ObjectManager& m_objectMng;
    Grid& m_grid;
    RuleSystem& m_ruleSystem;
    const Input& m_input;

    float m_moveTimer = 0.0f;
    bool m_movedLastFrame = false;
};