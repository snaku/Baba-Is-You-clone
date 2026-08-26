#pragma once

#include "game/object.hpp"

// std
#include <filesystem>
#include <sstream>

struct LevelObjectData
{
    ObjectId id = ObjectId::NONE;
    Cell cell = {0, 0};
};

struct LevelDefinition
{
    bool isValid = false;
    std::vector<LevelObjectData> objects;
    uint32_t width = 0;
    uint32_t height = 0;
};

class LevelFile
{
public:
    static LevelDefinition read(const std::filesystem::path& path);
    static bool write(const std::filesystem::path& path, const LevelDefinition& def);

private:
    static bool tryParseMetadata(LevelDefinition& def, std::stringstream& stream, const std::string& name);
};