#pragma once

#include "game/rule.hpp"
#include "game/ruleParser.hpp"
#include "game/object.hpp"

// std
#include <unordered_map>
#include <bitset>

class Level;

class RuleSystem
{
public:
    explicit RuleSystem(Level& level);

    bool update();

    void clear();

    void eraseObjectWithTransformation(std::size_t uid);

    void addBehavior(ObjectId id, BehaviorType behavior);
    void clearBehaviors(ObjectId id);
    bool hasBehavior(ObjectId id, BehaviorType behavior);

    void requestDirty() { m_dirty = true; }

private:
    void applyRules();
    void addToTransformationQueue(ObjectId id, ObjectId newId);
    void applyObjectsTransformation();
    void revertObjectsTransformation();

    Level& m_level;
    RuleParser m_parser;
    std::vector<Rule> m_rules;
    std::unordered_map<ObjectId, std::bitset<(std::size_t)BehaviorType::MAX>> m_behaviors;
    std::unordered_map<std::size_t, ObjectId> m_objectsWithTransformation;

    bool m_dirty = false;
};