#include "game/objectUtils.hpp"

namespace ObjectUtils
{

ObjectId textIdToNounId(ObjectId textId)
{
    if (getTypeFromId(textId) != ObjectType::TEXT ||
        getTextTypeFromId(textId) != TextType::NOUN)
    {
        return ObjectId::NONE;
    }

    switch (textId)
    {
        case ObjectId::TEXT_BABA:  return ObjectId::BABA;
        case ObjectId::TEXT_FLAG:  return ObjectId::FLAG;
        case ObjectId::TEXT_ROCK:  return ObjectId::ROCK;
        case ObjectId::TEXT_WATER: return ObjectId::WATER;
        case ObjectId::TEXT_WALL:  return ObjectId::WALL;
    }

    return ObjectId::NONE;
}

BehaviorType textIdToBehavior(ObjectId textId)
{
    if (getTypeFromId(textId) != ObjectType::TEXT ||
        getTextTypeFromId(textId) != TextType::BEHAVIOR)
    {
        return BehaviorType::NONE;
    }

    switch (textId)
    {
        case ObjectId::TEXT_YOU:  return BehaviorType::YOU;
        case ObjectId::TEXT_WIN:  return BehaviorType::WIN;
        case ObjectId::TEXT_SINK: return BehaviorType::SINK;
        case ObjectId::TEXT_STOP: return BehaviorType::STOP;
        case ObjectId::TEXT_PUSH: return BehaviorType::PUSH;
    }

    return BehaviorType::NONE;
}

ObjectType getTypeFromId(ObjectId id)
{
    switch (id)
    {
        case ObjectId::BABA:  [[fallthrough]];
        case ObjectId::WALL:  [[fallthrough]];
        case ObjectId::WATER: [[fallthrough]];
        case ObjectId::FLAG:  [[fallthrough]];
        case ObjectId::ROCK:  return ObjectType::ENTITY;

        case ObjectId::TEXT_BABA:  [[fallthrough]];
        case ObjectId::TEXT_WATER: [[fallthrough]];
        case ObjectId::TEXT_IS:    [[fallthrough]];
        case ObjectId::TEXT_NOT:   [[fallthrough]];
        case ObjectId::TEXT_AND:   [[fallthrough]];
        case ObjectId::TEXT_YOU:   [[fallthrough]];
        case ObjectId::TEXT_WIN:   [[fallthrough]];
        case ObjectId::TEXT_SINK:  [[fallthrough]];
        case ObjectId::TEXT_STOP:  [[fallthrough]];
        case ObjectId::TEXT_PUSH:  [[fallthrough]];
        case ObjectId::TEXT_FLAG:  [[fallthrough]];
        case ObjectId::TEXT_ROCK:  [[fallthrough]];
        case ObjectId::TEXT_WALL:  return ObjectType::TEXT;
    }

    return ObjectType::NONE;
}

TextType getTextTypeFromId(ObjectId textId)
{
    switch (textId)
    {
        case ObjectId::TEXT_BABA:  [[fallthrough]];
        case ObjectId::TEXT_FLAG:  [[fallthrough]];
        case ObjectId::TEXT_WATER: [[fallthrough]];
        case ObjectId::TEXT_ROCK:  [[fallthrough]];
        case ObjectId::TEXT_WALL:  return TextType::NOUN;

        case ObjectId::TEXT_IS:  [[fallthrough]];
        case ObjectId::TEXT_AND: [[fallthrough]];
        case ObjectId::TEXT_NOT: return TextType::OPERATOR;

        case ObjectId::TEXT_YOU:  [[fallthrough]];
        case ObjectId::TEXT_WIN:  [[fallthrough]];
        case ObjectId::TEXT_SINK: [[fallthrough]];
        case ObjectId::TEXT_STOP: [[fallthrough]];
        case ObjectId::TEXT_PUSH: return TextType::BEHAVIOR;
    }

    return TextType::NONE;
}

};