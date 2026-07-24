#include "game/objectUtils.hpp"

// std
#include <unordered_map>

namespace ObjectUtils
{

static const std::unordered_map<std::string, ObjectId> s_nameIdMap = 
{
    {"baba", ObjectId::BABA},
    {"wall", ObjectId::WALL},
    {"water", ObjectId::WATER},
    {"flag", ObjectId::FLAG},
    {"rock", ObjectId::ROCK},

    {"text_baba", ObjectId::TEXT_BABA},
    {"text_water", ObjectId::TEXT_WATER},
    {"text_is", ObjectId::TEXT_IS},
    {"text_not", ObjectId::TEXT_NOT},
    {"text_and", ObjectId::TEXT_AND},
    {"text_you", ObjectId::TEXT_YOU},
    {"text_win", ObjectId::TEXT_WIN},
    {"text_sink", ObjectId::TEXT_SINK},
    {"text_stop", ObjectId::TEXT_STOP},
    {"text_push", ObjectId::TEXT_PUSH},
    {"text_flag", ObjectId::TEXT_FLAG},
    {"text_rock", ObjectId::TEXT_ROCK},
    {"text_wall", ObjectId::TEXT_WALL},
};

ObjectId textIdToNounId(ObjectId textId)
{
    if (idToType(textId) != ObjectType::TEXT ||
        textIdToTextType(textId) != TextType::NOUN)
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
    if (idToType(textId) != ObjectType::TEXT ||
        textIdToTextType(textId) != TextType::BEHAVIOR)
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

ObjectType idToType(ObjectId id)
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

TextType textIdToTextType(ObjectId textId)
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

ObjectId stringToId(const std::string& name)
{
    auto it = s_nameIdMap.find(name);
    if (it == s_nameIdMap.end())
    {
        return ObjectId::NONE;
    }

    return it->second;
}

}