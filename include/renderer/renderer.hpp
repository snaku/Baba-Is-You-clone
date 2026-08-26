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
    void drawLine(SDL_FPoint start, float length, float angle, SDL_Color col);
    void drawTexture(SDL_Texture& texture, const SDL_FRect& rect, SDL_RendererFlip flip, float angle, SDL_Color col);

    void clear();
    void setClearColor(SDL_Color col) { m_clearCol = col; };

    void checkResize();

    SDL_Renderer* getSDLRenderer() const { return m_renderer; }
    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }
    bool wasResized() const { return m_wasResized; }
private:
    void resize(uint32_t width, uint32_t height);

    Window& m_window;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Color m_clearCol;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    bool m_wasResized = false;
};