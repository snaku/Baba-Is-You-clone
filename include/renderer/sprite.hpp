#pragma once

#include <SDL2/SDL.h>

// std
#include <filesystem>
#include <memory>

class Renderer;
class Texture;
class TextureManager;

struct SpriteInfo
{
    std::filesystem::path path;
    SDL_Color col;
};

class Sprite
{
public:
    Sprite(Renderer& renderer, TextureManager& textureMng, const SpriteInfo& info, const SDL_FPoint& basePos);
    ~Sprite() noexcept;

    void draw();

    void reload(const SpriteInfo& info);

    void setColor(SDL_Color col) { m_col = col; };
    void setFlip(SDL_RendererFlip flip) { m_flip = flip; };
    void setAngle(float angle) { m_angle = angle; };

    SDL_FPoint getPos() const { return m_pos; }
    void setPos(const SDL_FPoint& pos) { m_pos = pos; }

private:
    TextureManager& m_textureMng;
    std::shared_ptr<Texture> m_texture;
    Renderer& m_renderer;

    SDL_FPoint m_pos;
    SDL_RendererFlip m_flip = SDL_FLIP_NONE;
    float m_angle = 0.0f;
    SDL_Color m_col;
};