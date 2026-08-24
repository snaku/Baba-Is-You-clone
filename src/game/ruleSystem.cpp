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
    m_negatedBehaviors.clear();
    m_negatedObjectsTransformation.clear();
    m_negatedObjectsPossession.clear();
}

void RuleSystem::eraseObjectWithTransformation(std::size_t uid)
{
    m_objectsWithTransformation.erase(uid);
}

void RuleSystem::addBehavior(ObjectId id, BehaviorType behavior)
{
    m_behaviors[id].set(std::to_underlying(behavior));
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

    return it->second.test(std::to_underlying(behavior));
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

void RuleSystem::applyPredicate(ObjectId subject, BehaviorType behavior, bool _)
{
    auto it = m_negatedBehaviors.find(subject);
    if (it != m_negatedBehaviors.end())
    {
        if (it->second.test(std::to_underlying(behavior)))
        {
            std::println("REFUSE TO ADD BEHAVIOR ! NEGATED");
            return;
        }
    }

    addBehavior(subject, behavior);
}

void RuleSystem::applyPredicate(ObjectId subject, ObjectId newId, bool possessive)
{
    // everything possessive related is for HAS keyword prototype
    // which shouldn't be in this function

    if (!possessive)
    {
        auto it = m_negatedObjectsTransformation.find(subject);
        if (it != m_negatedObjectsTransformation.end())
        {
            if (it->second.test(std::to_underlying(newId)))
            {
                std::println("REFUSE TO TRANSFORM OBJECT ! NEGATED");
                return;
            }
        }

        addToTransformationQueue(subject, newId);
        return;
    }

    auto it = m_negatedObjectsPossession.find(subject);
    if (it != m_negatedObjectsPossession.end())
    {
        if (it->second.test(std::to_underlying(newId)))
        {
            std::println("REFUSE TO ADD OBJECT POSSESSION ! NEGATED");
            return;
        }
    }

    m_objectMng.forEach(
        [&](Object& object)
        {
            if (object.getId() == subject)
            {
                object.setPossessedId(newId);
            }
        }
    );
}

void RuleSystem::applyNegatedPredicate(ObjectId subject, BehaviorType behavior, bool _)
{
    m_negatedBehaviors[subject].set(std::to_underlying(behavior));
}

void RuleSystem::applyNegatedPredicate(ObjectId subject, ObjectId newId, bool possessive)
{
    if (!possessive)
    {
        m_negatedObjectsTransformation[subject].set(std::to_underlying(newId));
        return;
    }

    m_negatedObjectsPossession[subject].set(std::to_underlying(newId));
}    

void RuleSystem::applyRule(const Rule& rule)
{
    if (rule.negate)
    {
        return;
    }

    for (const auto& subject : rule.subjects)
    {
        std::visit(
            [&](const auto& predicate)
            {
                applyPredicate(subject, predicate, rule.possessivePredicate);
            },
            rule.predicate
        );
    }
}

void RuleSystem::applyNegatedRule(const Rule& rule)
{
    if (!rule.negate)
    {
        return;
    }

    for (const auto& subject : rule.subjects)
    {
        std::visit(
            [&](const auto& predicate)
            {
                applyNegatedPredicate(subject, predicate, rule.possessivePredicate);
            },
            rule.predicate
        );
    }
}

void RuleSystem::applyRules()
{
    m_behaviors.clear();
    m_negatedBehaviors.clear();
    m_negatedObjectsTransformation.clear();

    revertObjectsTransformation();

    for (const auto& rule : m_rules)
    {
        applyNegatedRule(rule);
    }
    
    for (const auto& rule : m_rules)
    {
        applyRule(rule);
    }

    applyObjectsTransformation();
}