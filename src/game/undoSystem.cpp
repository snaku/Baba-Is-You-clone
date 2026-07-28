#include "game/undoSystem.hpp"
#include "game/objectManager.hpp"

#include "time/time.hpp"

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

    m_history.pop(); // pop first because the top will always be the level current state

    const LevelSnapshot& snapshot = m_history.top();
    for (const auto& objectSnapshot : snapshot.objectSnapshots)
    {
        m_objectMng.addObjectFromUID(objectSnapshot.uid,
                                     objectSnapshot.id,
                                     objectSnapshot.cell);
    }
}

bool UndoSystem::updateTimer()
{
    m_timer += Time::deltaTime();

    return m_timer > s_delay;
}

void UndoSystem::clearTimer()
{
    m_timer = 0.0f;
}

void UndoSystem::clear()
{
    while (!m_history.empty())
    {
        m_history.pop();
    }

    clearTimer();
}