#include "game/saveSystem.hpp"

// std
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <print>

void SaveSystem::save(uint32_t levelId)
{
    if (hasData())
    {
        std::println("Overwriting old save file");
    }

    if (!std::filesystem::exists(getDirectory()))
    {
        std::filesystem::create_directory(getDirectory());
    }

    std::ofstream file(getPath());
    
    file << "level ";
    file << levelId;
}

Save SaveSystem::load()
{
    if (!hasData())
    {
        return {};
    }

    std::ifstream file(getPath());
    if (!file.is_open())
    {
        return {};
    }

    Save save{};
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream sstream(line);

        std::string levelStr;
        sstream >> levelStr;

        std::ranges::transform(levelStr, levelStr.begin(),
            [](char c)
            {
                return std::tolower(c);
            }
        );

        // i don't think there should be other data in the future ?

        if (levelStr == "level")
        {
            uint32_t levelId;
            sstream >> levelId;

            save.levelId = levelId;
        }
    }

    std::println("SAVE DATA OK: {}", getPath().string());

    return save;
}

bool SaveSystem::hasData()
{
    return std::filesystem::is_regular_file(getPath());
}

std::filesystem::path SaveSystem::getDirectory()
{
    std::filesystem::path localAppData = std::getenv("LOCALAPPDATA");

    return localAppData / "BabaIsYouClone";
}

std::filesystem::path SaveSystem::getPath()
{
    // path should be "C:/Users/../AppData/Local/BabaIsYouClone/save.txt"
    return getDirectory() / "save.txt";
}