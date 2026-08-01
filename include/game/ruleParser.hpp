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
#include <optional>
#include <span>

class ObjectManager;
class Grid;

class RuleParser
{
public:
    RuleParser(ObjectManager& objectMng, const Grid& grid);

    std::vector<Rule> parse();

private:
    void parseInDir(Direction dir);
    void parseFromNoun(Object& nounText);
    std::optional<std::vector<ObjectId>> parseANDOperatorForNouns(Object& baseNounText, Object*& lastTextNoun);
    void parseANDOperatorForPredicate(std::span<const ObjectId> textIds, Object& basePredicateText, bool possessivePredicate);

    Object* findText(std::span<const std::unique_ptr<Object>> objects, TextType type);
    Object* findText(std::span<Object*> objectsAt, TextType type);
    Object* findNextText(TextType type, Cell baseCell);

    Object* findAssignmentOp(const Object& nounText, bool& possessivePredicate);
    Object* findOpAND(const Object& text);
    Object* findOpNOT(const Object& op, bool& negate);
    Object* findPredicate(const Object& op, std::variant<ObjectId, BehaviorType>& predicate, bool possessivePredicate);

    void createRule(std::span<const ObjectId> subjects, std::variant<ObjectId, BehaviorType> predicate, bool negate, bool possessivePredicate);

    ObjectManager& m_objectMng;
    const Grid& m_grid;
    std::vector<Rule> m_rules;
    std::unordered_set<std::size_t> m_parsedNounsUID;
    Direction m_dir;
};