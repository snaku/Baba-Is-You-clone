#pragma once

// std
#include <memory>
#include <unordered_map>
#include <filesystem>

class Texture;
class Renderer;

class TextureManager
{
public:
    explicit TextureManager(Renderer& renderer);

    std::shared_ptr<Texture> getTexture(const std::filesystem::path& path);
    std::shared_ptr<Texture> getFromCache(const std::filesystem::path& path);
    void clearCache();
    
private:
    Renderer& m_renderer;
    std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> m_cache;
};