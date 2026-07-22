#pragma once

#include "game/object.hpp"

// std
#include <variant>

struct Rule
{
    ObjectId subject;
    std::variant<ObjectId, BehaviorType> predicate;
    bool negate = false;
};