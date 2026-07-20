#include "game/ruleParser.hpp"
#include "game/level.hpp"
#include "game/utils.hpp"

// std
#include <algorithm>
#include <vector>
#include <iostream>

RuleParser::RuleParser(Level& level) 
    : m_level(level)
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

    while (Object* nounText = findText(m_level.getObjects(), TextType::NOUN))
    {
        parseFromNoun(*nounText);
    }
}

void RuleParser::parseFromNoun(const Object& nounText)
{
    m_parsedNounsUID.insert(nounText.getUID());

    Object* opIS = findNextText(TextType::OPERATOR, nounText.getCell());
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

    createRule(nounText.textIdToNounId(), predicate, negate);

    parseANDOperator(nounText, *predicateText);
}

void RuleParser::parseANDOperator(const Object& nounText, Object& basePredicateText)
{
    Object* currentPredicateText = &basePredicateText;

    while (currentPredicateText != nullptr)
    {
        Object* opAND = findNextText(TextType::OPERATOR, currentPredicateText->getCell());
        if (opAND == nullptr ||
            opAND->getId() != ObjectId::TEXT_AND)
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

        createRule(nounText.textIdToNounId(), predicate, negate);
    }
}

Object* RuleParser::findText(const std::vector<std::unique_ptr<Object>>& objects, TextType type)
{
    auto it = std::find_if(objects.begin(), objects.end(),
    [&](const std::unique_ptr<Object>& object)
    {
        return object->isText() &&
               object->getTextType() == type &&
               m_parsedNounsUID.find(object->getUID()) == m_parsedNounsUID.end() &&
               object->getCell().isValidPos();
    });

    if (it == objects.end())
    {
        return nullptr;
    }

    return it->get();
}

Object* RuleParser::findText(const std::vector<Object*>& objectsAt, TextType type)
{
    auto it = std::find_if(objectsAt.begin(), objectsAt.end(),
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

    std::vector<Object*> texts;
    m_level.getObjectsAt(nextCell, texts);

    return findText(texts, type);
}

Object* RuleParser::findOpNOT(Object& op, bool& negate)
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

Object* RuleParser::findPredicate(Object& op,
                                  std::variant<ObjectId, BehaviorType>& predicate)
{
    Object* behaviorPredicate = findNextText(TextType::BEHAVIOR, op.getCell());
    if (behaviorPredicate != nullptr)
    {
        predicate = behaviorPredicate->textIdToBehavior();
        return behaviorPredicate;
    }

    Object* nounPredicate = findNextText(TextType::NOUN, op.getCell());
    if (nounPredicate != nullptr)
    {
        predicate = nounPredicate->textIdToNounId();
        return nounPredicate;
    }

    return nullptr;
}

void RuleParser::createRule(ObjectId subject,
                           std::variant<ObjectId, BehaviorType> predicate,
                           bool negate)
{
    m_rules.push_back({subject, predicate, negate});
}