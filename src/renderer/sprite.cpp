#include "renderer/sprite.hpp"
#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"
#include "renderer/textureManager.hpp"

Sprite::Sprite(Renderer& renderer, TextureManager& textureMng, const std::filesystem::path& texturePath, const SDL_FPoint& basePos) 
    : m_renderer(renderer),
      m_textureMng(textureMng),
      m_pos(basePos)
{
    m_texture = m_textureMng.getTexture(texturePath);
    m_col = {255, 255, 255, 255};
}
Sprite::~Sprite() noexcept
{
}

void Sprite::draw()
{
    if (m_texture != nullptr)
    {
        m_texture->drawAt(m_pos, m_flip, m_angle, m_col);
    }
}

void Sprite::reload(const std::filesystem::path& texturePath)
{
    m_texture = m_textureMng.getTexture(texturePath);
}