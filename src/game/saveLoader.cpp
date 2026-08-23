#include "game/saveSystem.hpp"

// std
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <print>

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

std::filesystem::path SaveSystem::getPath()
{
    std::filesystem::path localAppData = std::getenv("LOCALAPPDATA");

    // path should be "C:/Users/../AppData/Local/BabaIsYouClone/save.txt"
    return localAppData / "BabaIsYouClone" / "save.txt";
}