#pragma once

#include "game/object.hpp"

class Input;
class Grid;
class ObjectManager;
class RuleSystem;

class MovementSystem
{
public:
    MovementSystem(ObjectManager& objectMng, Grid& grid, RuleSystem& ruleSystem);

    bool updateMoveTimer();
    bool tryMoveYou(Object& object, const Input& input);
private:
    bool handleSinkInteraction(Object& object, Object& other);
    bool handlePushInteraction(Object& object, Direction dir);
    bool handleObjectInteractionsAt(Object& object, Cell cell, Direction dir);

    bool tryMove(Object& object, Direction dir);

    ObjectManager& m_objectMng;
    Grid& m_grid;
    RuleSystem& m_ruleSystem;

    float m_moveTimer = 0.0f;
    bool m_movedLastFrame = false;

    static constexpr float s_delay = 0.12f;
};