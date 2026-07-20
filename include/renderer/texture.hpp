#pragma once

#include <SDL2/SDL.h>

#include <filesystem>

class Renderer;

class Texture
{
public:
    explicit Texture(Renderer& renderer);
    Texture(Renderer& renderer, const std::filesystem::path& path);
    ~Texture() noexcept;

    bool load(const std::filesystem::path& path);
    void drawAt(const SDL_FPoint& pos, SDL_RendererFlip flip, float angle, SDL_Color col);

    SDL_Texture* getSDLTexture() const { return m_texture; }

private:
    SDL_Texture* m_texture = nullptr;
    Renderer& m_renderer;

    int m_width = 0;
    int m_height = 0;
};