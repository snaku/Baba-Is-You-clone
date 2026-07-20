#pragma once

#include "game/object.hpp"

// std
#include <variant>

struct Rule
{
    ObjectId subject;
    // BehaviorType predicate;
    std::variant<ObjectId, BehaviorType> predicate;
    bool negate = false;
};