#include "renderer/sprite.hpp"
#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"
#include "renderer/textureManager.hpp"

Sprite::Sprite(Renderer& renderer, TextureManager& textureMng, const SpriteInfo& info, const SDL_FPoint& basePos) 
    : m_renderer(renderer),
      m_textureMng(textureMng),
      m_pos(basePos)
{
    m_texture = m_textureMng.getTexture(info.path);
    m_col = info.col;
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

void Sprite::reload(const SpriteInfo& info)
{
    m_texture = m_textureMng.getTexture(info.path);
    m_col = info.col;
}