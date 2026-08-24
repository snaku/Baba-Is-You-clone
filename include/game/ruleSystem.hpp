#pragma once

#include "game/rule.hpp"
#include "game/ruleParser.hpp"
#include "game/object.hpp"

// std
#include <unordered_map>
#include <bitset>
#include <utility>

class Grid;
class ObjectManager;

class RuleSystem
{
public:
    RuleSystem(ObjectManager& objectMng, const Grid& grid);

    bool update();

    void clear();

    void eraseObjectWithTransformation(std::size_t uid);

    void addBehavior(ObjectId id, BehaviorType behavior);
    bool hasBehavior(ObjectId id, BehaviorType behavior) const;

    void requestDirty() { m_dirty = true; }

private:
    template<typename T>
    requires (std::is_scoped_enum_v<T> &&
              requires { T::MAX; }
    )
    using RuleBitsetMap = std::unordered_map<ObjectId, std::bitset<std::to_underlying(T::MAX)>>;

    template<typename T>
    bool bitsetMapContains(const RuleBitsetMap<T>& bitsetMap, ObjectId subject, T value) const
    {
        auto it = bitsetMap.find(subject);
        if (it == bitsetMap.end())
        {
            return false;
        }

        return it->second.test(std::to_underlying(value));
    }

    void applyPredicate(ObjectId subject, BehaviorType behavior, bool _);
    void applyPredicate(ObjectId subject, ObjectId newId, bool possessive);

    void applyNegatedPredicate(ObjectId subject, BehaviorType behavior, bool _);
    void applyNegatedPredicate(ObjectId subject, ObjectId newId, bool possessive);

    void applyRule(const Rule& rule);
    void applyNegatedRule(const Rule& rule);
    void applyRules();

    void addToTransformationQueue(ObjectId id, ObjectId newId);
    void applyObjectsTransformation();
    void revertObjectsTransformation();

    ObjectManager& m_objectMng;
    RuleParser m_parser;
    std::vector<Rule> m_rules;

    RuleBitsetMap<BehaviorType> m_behaviors;
    std::unordered_map<std::size_t, ObjectId> m_objectsWithTransformation;

    RuleBitsetMap<BehaviorType> m_negatedBehaviors;
    RuleBitsetMap<ObjectId> m_negatedObjectsTransformation;
    RuleBitsetMap<ObjectId> m_negatedObjectsPossession;

    bool m_dirty = false;
};