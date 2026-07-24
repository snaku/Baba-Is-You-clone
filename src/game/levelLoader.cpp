#include "game/levelLoader.hpp"
#include "game/objectUtils.hpp"

// std
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

LevelDefinition LevelLoader::read(const std::filesystem::path& path)
{
    LevelDefinition def{};

    if (!std::filesystem::exists(path) ||
        path.extension() != ".txt")
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
        std::string name;
        Cell cell = {0, 0};

        stream >> name >> cell.x >> cell.y;

        std::transform(name.begin(), name.end(), name.begin(), 
        [](char c)
        {
            return std::tolower(c);
        });

        ObjectId id = ObjectUtils::stringToId(name);
        if (id == ObjectId::NONE)
        {
            return def;
        }

        if (!cell.isValidPos())
        {
            return def;
        }

        def.objects.push_back({id, cell});
    }

    def.isValid = !def.objects.empty();

    std::cout << "Level loaded: " << path << std::endl;

    return def;
}