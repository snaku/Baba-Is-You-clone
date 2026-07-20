#include "renderer/textureManager.hpp"
#include "renderer/texture.hpp"

#include <iostream>

TextureManager::TextureManager(Renderer& renderer) 
    : m_renderer(renderer)
{
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::filesystem::path& path)
{
    std::shared_ptr<Texture> texture = getFromCache(path);

    if (texture != nullptr)
    {
        std::cout << "Using texture cache for: " << path << std::endl;
        return texture;
    }

    texture = std::make_shared<Texture>(m_renderer, path);
    m_cache[path] = texture;

    return texture;
}

std::shared_ptr<Texture> TextureManager::getFromCache(const std::filesystem::path& path)
{
    auto it = m_cache.find(path);

    if (it == m_cache.end())
    {
        return nullptr;
    }

    return it->second;
}

void TextureManager::clearCache()
{
    m_cache.clear();
}