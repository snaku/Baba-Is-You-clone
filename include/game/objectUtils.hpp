#pragma once

#include "game/object.hpp"

// std
#include <string>
#include <string_view>

namespace ObjectUtils
{

ObjectId textIdToNounId(ObjectId textId);
BehaviorType textIdToBehavior(ObjectId textId);
ObjectType idToType(ObjectId id);
TextType textIdToTextType(ObjectId textId);
ObjectId stringToId(const std::string& name);
std::string_view idToString(ObjectId id);

}