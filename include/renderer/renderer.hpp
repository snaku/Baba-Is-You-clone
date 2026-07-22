#pragma once

// SDL2
#include <SDL2/SDL.h>

class Window;

class Renderer
{
public:
    explicit Renderer(Window& window);
    ~Renderer() noexcept;

    bool init();

    void draw();

    void drawRect(const SDL_Rect& rect, SDL_Color col);
    void drawTexture(SDL_Texture& texture, const SDL_FRect& rect, SDL_RendererFlip flip, float angle, SDL_Color col);

    void clear();
    void setClearColor(SDL_Color col) { m_clearCol = col; };

    SDL_Renderer* getSDLRenderer() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer = nullptr;
    SDL_Color m_clearCol;

    Window& m_window;
};