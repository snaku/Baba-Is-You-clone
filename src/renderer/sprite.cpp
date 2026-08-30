#include "renderer/sprite.hpp"
#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"
#include "renderer/textureManager.hpp"

#include "time/time.hpp"

Sprite::Sprite(Renderer& renderer, TextureManager& textureMng, const SpriteInfo& info, const SDL_FPoint& basePos) 
    : m_renderer(renderer),
      m_textureMng(textureMng),
      m_pos(basePos)
{
    for (const auto& path : info.paths)
    {
        m_textures.push_back(m_textureMng.getTexture(path));
    }

    m_textureCount = m_textures.size();

    m_col = info.col;
}
Sprite::~Sprite() noexcept
{
}

void Sprite::draw(float width, float height)
{
    if (m_textureCount == 0)
    {
        return;
    }

    m_animTimer += Time::deltaTime();
    if (m_animTimer >= s_animFrameDuration)
    {
        m_animTimer -= s_animFrameDuration;
        m_currentAnimFrame = (m_currentAnimFrame + 1) % m_textureCount;
    }

    auto& currentTexture = m_textures[m_currentAnimFrame];
    if (currentTexture != nullptr)
    {
        currentTexture->drawAt(m_pos,
                               width,
                               height,
                               m_flip,
                               m_angle,
                               m_col);
    }
}

void Sprite::reload(const SpriteInfo& info)
{
    m_textures.clear();
    m_textureCount = 0;

    for (const auto& path : info.paths)
    {
        m_textures.push_back(m_textureMng.getTexture(path));
    }

    m_textureCount = m_textures.size();

    m_col = info.col;
    m_currentAnimFrame = 0;
    m_animTimer = 0.0f;
}