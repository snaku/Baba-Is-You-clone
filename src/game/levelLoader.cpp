#include "game/levelLoader.hpp"

// std
#include <sstream>
#include <fstream>
#include <iostream>

LevelDefinition LevelLoader::read(const std::filesystem::path& path)
{
    LevelDefinition def;

    if (path.extension() != ".txt")
    {
        return def;
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        return def;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() ||
            line[0] == '#')
        {
            continue;
        }

        std::stringstream stream(line);
        uint32_t id = 0; // >> operator doesn't work with enum class
        Cell cell = {0, 0};

        stream >> id >> cell.x >> cell.y;

        if (id == (uint32_t)ObjectId::NONE ||
            id >= (uint32_t)ObjectId::MAX)
        {
            return def;
        }

        def.objects.push_back({(ObjectId)id, cell});
    }

    def.isValid = !def.objects.empty();

    std::cout << "Level loaded: " << path << std::endl;

    return def;
}