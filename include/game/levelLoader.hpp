#pragma once

#include "game/object.hpp"

// std
#include <filesystem>

struct LevelObjectData
{
    ObjectId id = ObjectId::NONE;
    Cell cell = {0, 0};
};

struct LevelDefinition
{
    bool isValid = false;
    std::vector<LevelObjectData> objects;
};

class LevelLoader
{
public:
    static LevelDefinition read(const std::filesystem::path& path);
};