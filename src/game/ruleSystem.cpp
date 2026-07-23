#include "game/ruleSystem.hpp"
#include "game/level.hpp"

// std
#include <iostream>

RuleSystem::RuleSystem(Level& level)
    : m_level(level),
      m_parser(level)
{
}

bool RuleSystem::update()
{
    if (!m_dirty)
    {
        return false;
    }

    std::cout << "Updating rules..." << std::endl;

    m_rules = m_parser.parse();
    applyRules();

    m_dirty = false;

    return true;
}

void RuleSystem::clear()
{
    m_rules.clear();
    m_behaviors.clear();
    m_objectsWithTransformation.clear();
}

void RuleSystem::eraseObjectWithTransformation(std::size_t uid)
{
    m_objectsWithTransformation.erase(uid);
}

void RuleSystem::addBehavior(ObjectId id, BehaviorType behavior)
{
    m_behaviors[id].set((std::size_t)behavior);
}

void RuleSystem::clearBehaviors(ObjectId id)
{
    m_behaviors[id].reset();
}

bool RuleSystem::hasBehavior(ObjectId id, BehaviorType behavior)
{
    auto it = m_behaviors.find(id);

    if (it == m_behaviors.end())
    {
        return false;
    }

    return it->second.test((std::size_t)behavior);
}

void RuleSystem::addToTransformationQueue(ObjectId id, ObjectId newId)
{
    if (newId == ObjectId::NONE)
    {
        return;
    }

    const std::vector<std::unique_ptr<Object>>& objects = m_level.getObjects();
    for (const auto& object : objects)
    {
        if (object->getId() != id)
        {
            continue;
        }

        m_objectsWithTransformation.insert({object->getUID(), newId});
    }
}

void RuleSystem::applyObjectsTransformation()
{
    for (auto& [uid, newId] : m_objectsWithTransformation)
    {
        Object* object = m_level.findObjectFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        object->changeTo(newId);
    }
}

void RuleSystem::revertObjectsTransformation()
{
    for (auto& [uid, _] : m_objectsWithTransformation)
    {
        Object* object = m_level.findObjectFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        object->changeBack();
    }

    m_objectsWithTransformation.clear();
}

void RuleSystem::applyRules()
{
    m_behaviors.clear();
    revertObjectsTransformation();

    for (const auto& rule : m_rules)
    {
        if (!rule.negate)
        {
            if (std::holds_alternative<BehaviorType>(rule.predicate))
            {
                addBehavior(rule.subject, std::get<BehaviorType>(rule.predicate));
            }
            else
            {
                addToTransformationQueue(rule.subject, std::get<ObjectId>(rule.predicate));
            }
        }
        else
        {
            std::cout << "Negated !" << std::endl;
        }
    }

    applyObjectsTransformation();
}