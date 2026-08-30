#include "game/objectUtils.hpp"

// std
#include <unordered_map>
#include <utility>

namespace ObjectUtils
{

static const std::array<SpriteInfo, std::to_underlying(ObjectId::MAX)> s_spriteInfos =
{
    SpriteInfo{{"NONE"}, {0, 0, 0, 0}}, // ObjectId::NONE

    SpriteInfo{{"assets/baba_0_1.png", // ObjectId::BABA
                "assets/baba_0_2.png",
                "assets/baba_0_3.png"},
               {255, 255, 255, 255}},
    SpriteInfo{{"assets/wall_0_1.png", // ObjectId::WALL
                "assets/wall_0_2.png",
                "assets/wall_0_3.png"},
               {41, 49, 65, 255}},
    SpriteInfo{{"assets/water_0_1.png", // ObjectId::WATER
                "assets/water_0_2.png",
                "assets/water_0_3.png"},
               {95, 157, 209, 255}},
    SpriteInfo{{"assets/flag_0_1.png", // ObjectId::FLAG
                "assets/flag_0_2.png",
                "assets/flag_0_3.png"},
               {237, 226, 133, 255}},
    SpriteInfo{{"assets/rock_0_1.png", // ObjectId::ROCK
                "assets/rock_0_2.png",
                "assets/rock_0_3.png"},
               {194, 158, 70, 255}},

    SpriteInfo{{"assets/text_baba_0_1.png", // ObjectId::TEXT_BABA
                "assets/text_baba_0_2.png",
                "assets/text_baba_0_3.png"},
               {217, 57, 106, 255}},
    SpriteInfo{{"assets/text_water_0_1.png", // ObjectId::TEXT_WATER
                "assets/text_water_0_2.png",
                "assets/text_water_0_3.png"},
               {95, 157, 209, 255}},
    SpriteInfo{{"assets/text_is_0_1.png", // ObjectId::TEXT_IS
                "assets/text_is_0_2.png",
                "assets/text_is_0_3.png"},
               {255, 255, 255, 255}},
    SpriteInfo{{"assets/text_has_0_1.png", // ObjectId::TEXT_HAS
                "assets/text_has_0_2.png",
                "assets/text_has_0_3.png"},
               {255, 255, 255, 255}},
    SpriteInfo{{"assets/text_and_0_1.png", // ObjectId::TEXT_AND
                "assets/text_and_0_2.png",
                "assets/text_and_0_3.png"},
               {255, 255, 255, 255}},
    SpriteInfo{{"assets/text_not_0_1.png", // ObjectId::TEXT_NOT
                "assets/text_not_0_2.png",
                "assets/text_not_0_3.png"},
               {229, 83, 59, 255}},
    SpriteInfo{{"assets/text_you_0_1.png", // ObjectId::TEXT_YOU
                "assets/text_you_0_2.png",
                "assets/text_you_0_3.png"},
               {217, 57, 106, 255}},
    SpriteInfo{{"assets/text_win_0_1.png", // ObjectId::TEXT_WIN
                "assets/text_win_0_2.png",
                "assets/text_win_0_3.png"},
               {237, 226, 133, 255}},
    SpriteInfo{{"assets/text_defeat_0_1.png", // ObjectId::TEXT_DEFEAT
                "assets/text_defeat_0_2.png",
                "assets/text_defeat_0_3.png"},
               {130, 38, 28, 255}},
    SpriteInfo{{"assets/text_sink_0_1.png", // ObjectId::TEXT_SINK
                "assets/text_sink_0_2.png",
                "assets/text_sink_0_3.png"},
               {95, 157, 209, 255}},
    SpriteInfo{{"assets/text_stop_0_1.png", // ObjectId::TEXT_STOP
                "assets/text_stop_0_2.png",
                "assets/text_stop_0_3.png"},
               {27, 92, 28, 255}},
    SpriteInfo{{"assets/text_push_0_1.png", // ObjectId::TEXT_PUSH
                "assets/text_push_0_2.png",
                "assets/text_push_0_3.png"},
               {144, 103, 62, 255}},
    SpriteInfo{{"assets/text_flag_0_1.png", // ObjectId::TEXT_FLAG
                "assets/text_flag_0_2.png",
                "assets/text_flag_0_3.png"},
               {237, 226, 133, 255}},
    SpriteInfo{{"assets/text_rock_0_1.png", // ObjectId::TEXT_ROCK
                "assets/text_rock_0_2.png",
                "assets/text_rock_0_3.png"},
               {144, 103, 62, 255}},
    SpriteInfo{{"assets/text_wall_0_1.png", // ObjectId::TEXT_WALL
                "assets/text_wall_0_2.png",
                "assets/text_wall_0_3.png"},
               {115, 115, 115, 255}},
};

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
    {"text_has", ObjectId::TEXT_HAS},
    {"text_not", ObjectId::TEXT_NOT},
    {"text_and", ObjectId::TEXT_AND},
    {"text_you", ObjectId::TEXT_YOU},
    {"text_win", ObjectId::TEXT_WIN},
    {"text_defeat", ObjectId::TEXT_DEFEAT},
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
        case ObjectId::TEXT_YOU:    return BehaviorType::YOU;
        case ObjectId::TEXT_WIN:    return BehaviorType::WIN;
        case ObjectId::TEXT_DEFEAT: return BehaviorType::DEFEAT;
        case ObjectId::TEXT_SINK:   return BehaviorType::SINK;
        case ObjectId::TEXT_STOP:   return BehaviorType::STOP;
        case ObjectId::TEXT_PUSH:   return BehaviorType::PUSH;
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

        case ObjectId::TEXT_BABA:   [[fallthrough]];
        case ObjectId::TEXT_WATER:  [[fallthrough]];
        case ObjectId::TEXT_IS:     [[fallthrough]];
        case ObjectId::TEXT_HAS:    [[fallthrough]];
        case ObjectId::TEXT_NOT:    [[fallthrough]];
        case ObjectId::TEXT_AND:    [[fallthrough]];
        case ObjectId::TEXT_YOU:    [[fallthrough]];
        case ObjectId::TEXT_WIN:    [[fallthrough]];
        case ObjectId::TEXT_DEFEAT: [[fallthrough]];
        case ObjectId::TEXT_SINK:   [[fallthrough]];
        case ObjectId::TEXT_STOP:   [[fallthrough]];
        case ObjectId::TEXT_PUSH:   [[fallthrough]];
        case ObjectId::TEXT_FLAG:   [[fallthrough]];
        case ObjectId::TEXT_ROCK:   [[fallthrough]];
        case ObjectId::TEXT_WALL:   return ObjectType::TEXT;
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
        case ObjectId::TEXT_HAS: [[fallthrough]];
        case ObjectId::TEXT_AND: [[fallthrough]];
        case ObjectId::TEXT_NOT: return TextType::OPERATOR;

        case ObjectId::TEXT_YOU:    [[fallthrough]];
        case ObjectId::TEXT_WIN:    [[fallthrough]];
        case ObjectId::TEXT_DEFEAT: [[fallthrough]];
        case ObjectId::TEXT_SINK:   [[fallthrough]];
        case ObjectId::TEXT_STOP:   [[fallthrough]];
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

std::string_view idToString(ObjectId id)
{
    for (const auto& [name, value] : s_nameIdMap)
    {
        if (value == id)
        {
            return name;
        }
    }

    return "";
}

const SpriteInfo& getSpriteInfo(ObjectId id)
{
    return s_spriteInfos.at(std::to_underlying(id));
}

}