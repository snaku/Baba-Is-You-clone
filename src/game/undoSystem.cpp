#include "game/undoSystem.hpp"
#include "game/objectManager.hpp"

UndoSystem::UndoSystem(ObjectManager& objectMng)
    : m_objectMng(objectMng)
{
}

void UndoSystem::snap()
{
    LevelSnapshot snapshot;

    m_objectMng.forEach(
        [&snapshot](const Object& object)
        {
            snapshot.objectSnapshots.push_back({object.getUID(),
                                                object.getId(),
                                                object.getCell()});
        }
    );

    m_history.push(std::move(snapshot));
}

void UndoSystem::undo()
{
    if (m_history.size() <= 1)
    {
        return;
    }

    m_objectMng.clear();

    m_history.pop();
    const LevelSnapshot& snapshot = m_history.top();
    for (const auto& objectSnapshot : snapshot.objectSnapshots)
    {
        m_objectMng.addObjectFromUID(objectSnapshot.uid,
                                     objectSnapshot.id,
                                     objectSnapshot.cell);
    }
}

void UndoSystem::clear()
{
    while (!m_history.empty())
    {
        m_history.pop();
    }
}