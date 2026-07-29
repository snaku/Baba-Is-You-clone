#include "game/ruleSystem.hpp"
#include "game/objectManager.hpp"

// std
#include <print>

RuleSystem::RuleSystem(ObjectManager& objectMng, const Grid& grid)
    : m_objectMng(objectMng),
      m_parser(objectMng, grid)
{
}

bool RuleSystem::update()
{
    if (!m_dirty)
    {
        return false;
    }

    std::println("Updating rules...");

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
    if (id == ObjectId::NONE ||
        newId == ObjectId::NONE)
    {
        return;
    }

    m_objectMng.forEach(
        [&](const Object& object)
        {
            if (object.getId() == id)
            {
                m_objectsWithTransformation.insert({object.getUID(), newId});
            }
        }
    );
}

void RuleSystem::applyObjectsTransformation()
{
    for (auto& [uid, newId] : m_objectsWithTransformation)
    {
        Object* object = m_objectMng.findFromUID(uid);
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
        Object* object = m_objectMng.findFromUID(uid);
        if (object == nullptr)
        {
            continue;
        }

        object->changeBack();
    }

    m_objectsWithTransformation.clear();
}

void RuleSystem::applyPredicate(ObjectId subject, BehaviorType behavior)
{
    addBehavior(subject, behavior);
}

void RuleSystem::applyPredicate(ObjectId subject, ObjectId newId)
{
    addToTransformationQueue(subject, newId);
}

void RuleSystem::applyRule(const Rule& rule)
{
    if (rule.negate)
    {
        // TODO
        std::println("Negated !");
        return;
    }

    for (const auto& subject : rule.subjects)
    {
        std::visit(
            [&](const auto& predicate)
            {
                applyPredicate(subject, predicate);
            },
            rule.predicate
        );
    }
}

void RuleSystem::applyRules()
{
    m_behaviors.clear();
    revertObjectsTransformation();

    for (const auto& rule : m_rules)
    {
        applyRule(rule);
    }

    applyObjectsTransformation();
}