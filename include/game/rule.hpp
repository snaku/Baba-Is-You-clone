#pragma once

#include "game/object.hpp"

// std
#include <vector>
#include <variant>

struct Rule
{
    std::vector<ObjectId> subjects;
    std::variant<ObjectId, BehaviorType> predicate;
    bool negate = false;
    bool possessivePredicate = false;
};