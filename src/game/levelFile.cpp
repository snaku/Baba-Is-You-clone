#include "game/levelFile.hpp"
#include "game/objectUtils.hpp"

// std
#include <fstream>
#include <print>
#include <algorithm>

LevelDefinition LevelFile::read(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path) ||
        path.extension() != s_extension)
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

        Cell cell;

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

bool LevelFile::tryParseMetadata(LevelDefinition& def,
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

bool LevelFile::write(const std::filesystem::path& path, const LevelDefinition& def)
{
    if (!def.isValid ||
        path.extension() != s_extension)
    {
        return false;
    }

    if (std::filesystem::exists(path))
    {
        std::println("Overwriting existing level file: {}", path.string());
    }

    std::ofstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    file << "# METADATA\n";
    file << "width " << def.width << '\n';
    file << "height " << def.height << "\n\n";

    file << "# OBJECTS\n";
    for (const auto& data : def.objects)
    {
        if (data.id == ObjectId::NONE ||
            !data.cell.isValidPos())
        {
            continue;
        }

        std::string_view name = ObjectUtils::idToString(data.id);
        if (name.empty())
        {
            continue;
        }

        file << name << ' ' << data.cell.x << ' ' << data.cell.y << '\n';
    }

    std::println("Level created: {}", path.string());

    return true;
}