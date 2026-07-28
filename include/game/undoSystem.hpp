#pragma once

#include "game/object.hpp"

// std
#include <stack>

class ObjectManager;

struct ObjectSnapshot
{
    std::size_t uid;
    ObjectId id;
    Cell cell;
};

struct LevelSnapshot
{
    std::vector<ObjectSnapshot> objectSnapshots;
};

class UndoSystem
{
public:
    explicit UndoSystem(ObjectManager& objectMng);

    void snap();
    void undo();
    bool updateTimer();
    void clearTimer();
    void clear();

private:
    ObjectManager& m_objectMng;
    std::stack<LevelSnapshot> m_history;
    float m_timer = 0.0f;

    static constexpr float s_delay = 0.15f;
};