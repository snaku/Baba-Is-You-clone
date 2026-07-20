#pragma once

#include "game/rule.hpp"
#include "game/cell.hpp"
#include "game/object.hpp"

// std
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <type_traits>

class Level;

class RuleParser
{
public:
    RuleParser(Level& level);

    std::vector<Rule> parse();

private:
    void parseInDir(Direction dir);
    void parseFromNoun(const Object& nounText);
    void parseANDOperator(const Object& nounText, Object& basePredicateText);

    Object* findText(const std::vector<std::unique_ptr<Object>>& objects, TextType type);
    Object* findText(const std::vector<Object*>& objectsAt, TextType type);
    Object* findNextText(TextType type, Cell baseCell);

    Object* findOpNOT(Object& op, bool& negate);
    Object* findPredicate(Object& op, std::variant<ObjectId, BehaviorType>& predicate);

    void createRule(ObjectId subject, std::variant<ObjectId, BehaviorType> predicate, bool negate);

    Level& m_level;
    std::vector<Rule> m_rules;
    std::unordered_set<std::size_t> m_parsedNounsUID;
    Direction m_dir;
};