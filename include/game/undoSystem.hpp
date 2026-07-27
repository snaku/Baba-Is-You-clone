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
    void clear();

private:
    ObjectManager& m_objectMng;
    std::stack<LevelSnapshot> m_history;
};