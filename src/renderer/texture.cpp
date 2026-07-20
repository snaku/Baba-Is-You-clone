#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"

#include <SDL2/SDL_image.h>

Texture::Texture(Renderer& renderer) : m_renderer(renderer)
{
}
Texture::Texture(Renderer& renderer, const std::filesystem::path& path) : m_renderer(renderer)
{
    load(path);
}
Texture::~Texture() noexcept
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
    }
}

bool Texture::load(const std::filesystem::path& path)
{
    m_texture = IMG_LoadTexture(m_renderer.getSDLRenderer(), path.string().c_str());

    if (m_texture == nullptr)
    {
        return false;
    }

    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_width, &m_height);

    return true;
}

void Texture::drawAt(const SDL_FPoint& pos,
                     SDL_RendererFlip flip,
                     float angle,
                     SDL_Color col)
{
    SDL_FRect rect =
    {
        pos.x,
        pos.y,
        (float)m_width,
        (float)m_height
    };

    SDL_SetTextureColorMod(m_texture, col.r, col.g, col.b);

    SDL_RenderCopyExF(m_renderer.getSDLRenderer(),
                      m_texture,
                      nullptr,
                      &rect,
                      angle,
                      nullptr,
                      flip);
}