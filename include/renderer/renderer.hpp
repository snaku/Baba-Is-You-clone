#pragma once

#include <SDL2/SDL.h>

class Window;

class Renderer
{
public:
    explicit Renderer(Window& window);
    ~Renderer() noexcept;

    bool init();

    void draw();

    void clear();
    void setClearColor(SDL_Color col) { m_clearCol = col; };

    SDL_Renderer* getSDLRenderer() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer = nullptr;
    SDL_Color m_clearCol;

    Window& m_window;
};