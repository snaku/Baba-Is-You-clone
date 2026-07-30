#include "game/ruleParser.hpp"
#include "game/objectManager.hpp"
#include "game/objectUtils.hpp"
#include "game/utils.hpp"
#include "game/grid.hpp"

// std
#include <algorithm>
#include <vector>
#include <iostream>
#include <print>

RuleParser::RuleParser(ObjectManager& objectMng, const Grid& grid) 
    : m_objectMng(objectMng),
      m_grid(grid)
{
}

std::vector<Rule> RuleParser::parse()
{
    m_rules.clear();

    parseInDir(Direction::RIGHT);
    parseInDir(Direction::DOWN);

    return m_rules;
}

void RuleParser::parseInDir(Direction dir)
{
    m_parsedNounsUID.clear();

    m_dir = dir;

    while (Object* nounText = findText(m_objectMng.getObjects(), TextType::NOUN))
    {
        parseFromNoun(*nounText);
    }
}

void RuleParser::parseFromNoun(Object& nounText)
{
    m_parsedNounsUID.insert(nounText.getUID());

    Object* lastNounText = &nounText;
    auto subjects = parseANDOperatorForNouns(nounText, lastNounText);
    if (subjects == std::nullopt)
    {
        return;
    }

    Object* opIS = findNextText(TextType::OPERATOR, lastNounText->getCell());
    if (opIS == nullptr ||
        opIS->getId() != ObjectId::TEXT_IS)
    {
        return;
    }

    bool negate = false;
    Object* opNOT = findOpNOT(*opIS, negate);

    std::variant<ObjectId, BehaviorType> predicate;
    Object* predicateText = findPredicate(opNOT != nullptr ? *opNOT : *opIS,
                                          predicate);
    if (predicateText == nullptr)
    {
        return;
    }

    createRule(subjects.value(), predicate, negate);

    parseANDOperatorForPredicate(subjects.value(), *predicateText);
}

std::optional<std::vector<ObjectId>> RuleParser::parseANDOperatorForNouns(Object& baseNounText, Object*& lastTextNoun)
{
    std::vector<ObjectId> subjects;
    Object* currentNounText = &baseNounText;
    std::size_t opANDCount = 0;

    while (currentNounText != nullptr)
    {
        subjects.push_back(ObjectUtils::textIdToNounId(currentNounText->getId()));

        Object* opAND = findOpAND(*currentNounText);
        if (opAND == nullptr)
        {
            break;
        }

        opANDCount++;

        currentNounText = findNextText(TextType::NOUN, opAND->getCell());
        if (currentNounText == nullptr)
        {
            break;
        }

        m_parsedNounsUID.insert(currentNounText->getUID());
    }

    // a valid chain with n subjects needs n - 1 AND operators
    if (subjects.size() != opANDCount + 1)
    {
        return std::nullopt;
    }

    if (currentNounText == nullptr)
    {
        return std::nullopt;
    }

    lastTextNoun = currentNounText;

    return subjects;
}

void RuleParser::parseANDOperatorForPredicate(std::span<const ObjectId> nounsIds, 
                                              Object& basePredicateText)
{
    Object* currentPredicateText = &basePredicateText;

    while (currentPredicateText != nullptr)
    {
        Object* opAND = findOpAND(*currentPredicateText);
        if (opAND == nullptr)
        {
            break;
        }

        bool negate = false;
        Object* opNOT = findOpNOT(*opAND, negate);

        std::variant<ObjectId, BehaviorType> predicate;
        currentPredicateText = findPredicate(opNOT != nullptr ? *opNOT : *opAND,
                                             predicate);
        if (currentPredicateText == nullptr)
        {
            break;
        }

        for (auto id : nounsIds)
        {
            std::array<ObjectId, 1> arr{id}; // so i can pass it to createRule() std::span argument
            createRule(arr, predicate, negate);
        }
    }
}

Object* RuleParser::findText(std::span<const std::unique_ptr<Object>> objects, TextType type)
{
    auto it = std::ranges::find_if(objects,
        [&](const std::unique_ptr<Object>& object)
        {
            return object->isText() &&
                   object->getTextType() == type &&
                   !m_parsedNounsUID.contains(object->getUID()) &&
                   object->getCell().isValidPos();
        });

    if (it == objects.end())
    {
        return nullptr;
    }

    return it->get();
}

Object* RuleParser::findText(std::span<Object*> objectsAt, TextType type)
{
    auto it = std::ranges::find_if(objectsAt,
        [&](const Object* object)
        {
            return object->isText() &&
                   object->getTextType() == type &&
                   object->getCell().isValidPos();
        });

    if (it == objectsAt.end())
    {
        return nullptr;
    }

    return *it;
}

Object* RuleParser::findNextText(TextType type, Cell baseCell)
{
    Cell nextCell = GameUtils::getNextCellFromDir(baseCell, m_dir);
    if (!nextCell.isValidPos())
    {
        return nullptr;
    }

    // check if an operator is at the limit of the level
    // if so, then just stop parsing
    // because something like "BABA IS" isn't a valid rule
    if (type == TextType::OPERATOR &&
        !GameUtils::getNextCellFromDir(nextCell, m_dir).isValidPos())
    {
        return nullptr;
    }

    auto objects = m_objectMng.findFromUIDs(m_grid.getObjectsAt(nextCell));
    return findText(objects, type);
}

Object* RuleParser::findOpAND(const Object& text)
{
    Object* opAND = findNextText(TextType::OPERATOR, text.getCell());

    if (opAND == nullptr ||
        opAND->getId() != ObjectId::TEXT_AND)
    {
        return nullptr;
    }

    return opAND;
}

Object* RuleParser::findOpNOT(const Object& op, bool& negate)
{
    int count = 0;
    Cell nextCell = op.getCell();
    Object* last = nullptr;

    while (Object* current = findNextText(TextType::OPERATOR, nextCell))
    {
        if (current->getId() != ObjectId::TEXT_NOT)
        {
            break;
        }

        last = current;
        nextCell = current->getCell();
        count++;
    }

    negate = (count % 2) != 0;
    
    return last;
}

Object* RuleParser::findPredicate(const Object& op,
                                  std::variant<ObjectId, BehaviorType>& predicate)
{
    Object* behaviorPredicate = findNextText(TextType::BEHAVIOR, op.getCell());
    if (behaviorPredicate != nullptr)
    {
        predicate = ObjectUtils::textIdToBehavior(behaviorPredicate->getId());
        return behaviorPredicate;
    }

    Object* nounPredicate = findNextText(TextType::NOUN, op.getCell());
    if (nounPredicate != nullptr)
    {
        predicate = ObjectUtils::textIdToNounId(nounPredicate->getId());
        return nounPredicate;
    }

    return nullptr;
}

void RuleParser::createRule(std::span<const ObjectId> subjects,
                            std::variant<ObjectId, BehaviorType> predicate,
                            bool negate)
{
    m_rules.push_back({{subjects.begin(), subjects.end()}, predicate, negate});
}