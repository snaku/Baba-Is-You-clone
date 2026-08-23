#pragma once

// std
#include <filesystem>
#include <cstdint>
#include <optional>

struct Save
{
    std::optional<uint32_t> levelId;
};

class SaveSystem
{
public:
    static void save(uint32_t levelId);
    static Save load();
    static bool hasData();

private:
    static std::filesystem::path getDirectory();
    static std::filesystem::path getPath();
};