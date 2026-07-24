#pragma once

#include "game/object.hpp"

namespace ObjectUtils
{

ObjectId textIdToNounId(ObjectId textId);
BehaviorType textIdToBehavior(ObjectId textId);
ObjectType getTypeFromId(ObjectId id);
TextType getTextTypeFromId(ObjectId textId);

};