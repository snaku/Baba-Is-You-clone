#include "game/levelLoader.hpp"
#include "game/objectUtils.hpp"

// std
#include <fstream>
#include <print>
#include <algorithm>

LevelDefinition LevelLoader::read(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path) ||
        path.extension() != ".txt")
    {
        return {};
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        return {};
    }

    LevelDefinition def{};
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
        stream >> name;

        std::ranges::transform(name, name.begin(),
            [](char c)
            {
                return std::tolower(c);
            });

        if (tryParseMetadata(def, stream, name))
        {
            continue;
        }

        Cell cell = {0, 0};

        stream >> cell.x >> cell.y;

        ObjectId id = ObjectUtils::stringToId(name);
        if (id == ObjectId::NONE)
        {
            return {};
        }

        def.objects.push_back({id, cell});
    }

    def.isValid = !def.objects.empty() &&
                  def.width > 0 &&
                  def.height > 0;

    std::println("Level loaded: {}", path.string());

    return def;
}

bool LevelLoader::tryParseMetadata(LevelDefinition& def,
                                   std::stringstream& stream,
                                   const std::string& name)
{
    if (def.width == 0 &&
        name == "width")
    {
        stream >> def.width;
        return true;
    }
    else if (def.height == 0 &&
             name == "height")
    {
        stream >> def.height;
        return true;
    }

    return false;
}