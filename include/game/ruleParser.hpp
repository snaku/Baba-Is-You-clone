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
    void parseANDOperatorForPredicate(const std::vector<ObjectId>& textIds, Object& basePredicateText);

    Object* findText(const std::vector<std::unique_ptr<Object>>& objects, TextType type);
    Object* findText(const std::vector<Object*>& objectsAt, TextType type);
    Object* findNextText(TextType type, Cell baseCell);

    Object* findOpAND(const Object& text);
    Object* findOpNOT(const Object& op, bool& negate);
    Object* findPredicate(const Object& op, std::variant<ObjectId, BehaviorType>& predicate);

    void createRule(const std::vector<ObjectId>& subjects, std::variant<ObjectId, BehaviorType> predicate, bool negate);

    ObjectManager& m_objectMng;
    const Grid& m_grid;
    std::vector<Rule> m_rules;
    std::unordered_set<std::size_t> m_parsedNounsUID;
    Direction m_dir;
};