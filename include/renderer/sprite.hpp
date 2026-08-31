#pragma once

// SDL2
#include <SDL2/SDL.h>

// std
#include <filesystem>
#include <memory>
#include <vector>

class Renderer;
class Texture;
class TextureManager;

struct SpriteInfo
{
    std::vector<std::filesystem::path> paths;
    SDL_Color col;
};

class Sprite
{
public:
    Sprite(Renderer& renderer, TextureManager& textureMng, const SpriteInfo& info, const SDL_FPoint& basePos);
    ~Sprite() noexcept;

    void updateAnim();
    void draw(float width, float height);

    void reload(const SpriteInfo& info);

    void setColor(SDL_Color col) { m_col = col; };
    void setFlip(SDL_RendererFlip flip) { m_flip = flip; };
    void setAngle(float angle) { m_angle = angle; };

    SDL_FPoint getPos() const { return m_pos; }
    void setPos(const SDL_FPoint& pos) { m_pos = pos; }

    SDL_Color getColor() const { return m_col; }

private:
    TextureManager& m_textureMng;
    std::vector<std::shared_ptr<Texture>> m_textures;
    std::size_t m_textureCount = 0;
    Renderer& m_renderer;

    SDL_FPoint m_pos;
    SDL_RendererFlip m_flip = SDL_FLIP_NONE;
    float m_angle = 0.0f;
    SDL_Color m_col;
    uint32_t m_currentAnimFrame = 0;
    float m_animTimer = 0.0f;

    static constexpr float s_animFrameDuration = 0.15f; // 150ms
};