#include "game/movementSystem.hpp"
#include "game/level.hpp"
#include "game/grid.hpp"
#include "game/ruleSystem.hpp"
#include "game/utils.hpp"

#include "input/input.hpp"

#include "time/time.hpp"

MovementSystem::MovementSystem(Level& level, Grid& grid, RuleSystem& ruleSystem, const Input& input)
    : m_level(level),
      m_grid(grid),
      m_ruleSystem(ruleSystem),
      m_input(input)
{
}

bool MovementSystem::updateMoveTimer()
{
    m_moveTimer += Time::deltaTime();

    if (!m_movedLastFrame)
    {
        m_moveTimer = 0.10f;
    }

    if (m_moveTimer >= 0.10f)
    {
        m_moveTimer -= 0.10f;
        return true;
    }

    return false;
}

bool MovementSystem::tryMoveYou(Object& object)
{
    if (!m_ruleSystem.hasBehavior(object.getId(), BehaviorType::YOU))
    {
        return false;
    }

    Direction dir = Direction::NONE;

    if (m_input.isKeyDown(SDL_SCANCODE_W))
    {
        dir = Direction::UP;
    }
    else if (m_input.isKeyDown(SDL_SCANCODE_S))
    {
        dir = Direction::DOWN;
    }
    else if (m_input.isKeyDown(SDL_SCANCODE_A))
    {
        dir = Direction::LEFT;
    }
    else if (m_input.isKeyDown(SDL_SCANCODE_D))
    {
        dir = Direction::RIGHT;
    }

    if (dir == Direction::NONE)
    {
        m_movedLastFrame = false;
        return false;
    }

    m_movedLastFrame = true;

    object.faceDir(dir);

    return tryMove(object, dir);
}

bool MovementSystem::handleSinkInteraction(Object& object, Object& other)
{
    if (!m_ruleSystem.hasBehavior(object.getId(), BehaviorType::SINK) &&
        !m_ruleSystem.hasBehavior(other.getId(), BehaviorType::SINK))
    {
        return false;
    }

    object.requestKill();
    other.requestKill();

    return true;
}

bool MovementSystem::handlePushInteraction(Object& object, Direction dir)
{
    if (!m_ruleSystem.hasBehavior(object.getId(), BehaviorType::PUSH) &&
        !object.isText())
    {
        return true; // objects without PUSH behavior can be passed through
    }

    Cell nextCell = GameUtils::getNextCellFromDir(object.getCell(), dir);
    if (!nextCell.isValidPos() ||
        nextCell == object.getCell())
    {
        return false;
    }

    if (!tryMove(object, dir))
    {
        return false;
    }

    if (object.isText())
    {
        m_ruleSystem.requestDirty();
    }

    return true;
}

bool MovementSystem::handleObjectInteractionsAt(Object& object, Cell cell, Direction dir)
{
    std::vector<Object*> others;
    m_level.getObjectsAt(cell, others);

    for (auto& other : others)
    {
        if (other == nullptr)
        {
            continue;
        }

        if (handleSinkInteraction(object, *other) ||
            m_ruleSystem.hasBehavior(other->getId(), BehaviorType::STOP) ||
            !handlePushInteraction(*other, dir))
        {
            return false;
        }
    }

    return true;
}

bool MovementSystem::tryMove(Object& object, Direction dir)
{
    Cell next = GameUtils::getNextCellFromDir(object.getCell(), dir);
    if (!next.isValidPos() ||
        next == object.getCell())
    {
        return false;
    }

    if (!handleObjectInteractionsAt(object, next, dir))
    {
        return false;
    }

    if (object.shouldGetKilled())
    {
        return true;
    }

    m_grid.removeObjectAt(object.getUID(), object.getCell());
    object.move(dir);
    m_grid.addObjectAt(object.getUID(), object.getCell());

    return true;
}