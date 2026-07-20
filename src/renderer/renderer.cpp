#include "renderer/renderer.hpp"
#include "window/window.hpp"

#include <SDL2/SDL_image.h>

Renderer::Renderer(Window& window) : m_window(window)
{
}
Renderer::~Renderer() noexcept
{
    IMG_Quit();

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
    }
}

bool Renderer::init()
{
    m_renderer = SDL_CreateRenderer(m_window.getSDLWindow(),
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        return false;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    m_clearCol = {0, 0, 0, 255};
    return true;
}

void Renderer::draw()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, m_clearCol.r, m_clearCol.g, m_clearCol.b, m_clearCol.a);
    SDL_RenderClear(m_renderer);
}