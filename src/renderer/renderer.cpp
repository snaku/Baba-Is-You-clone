#include "renderer/renderer.hpp"
#include "window/window.hpp"

// SDL2
#include <SDL2/SDL_image.h>

// std
#include <cmath>
#include <numbers>

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

    m_width = m_window.getWidth();
    m_height = m_window.getHeight();

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Renderer::draw()
{
    SDL_RenderPresent(m_renderer);
}

void Renderer::drawRect(const SDL_Rect& rect, SDL_Color col)
{
    SDL_SetRenderDrawColor(m_renderer, col.r, col.g, col.b, col.a);
    SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::drawLine(SDL_FPoint start,
                        float length,
                        float angle,
                        SDL_Color col)
{
    constexpr float toRad = std::numbers::pi_v<float> / 180.0f;
    float rad = angle * toRad;

    SDL_FPoint end = SDL_FPoint
    {
        start.x + std::cos(rad) * length,
        start.y + std::sin(rad) * length
    };

    SDL_SetRenderDrawColor(m_renderer, col.r, col.g, col.b, col.a);
    SDL_RenderDrawLineF(m_renderer, start.x, start.y, end.x, end.y);
}

void Renderer::drawTexture(SDL_Texture& texture,
                           const SDL_FRect& rect,
                           SDL_RendererFlip flip,
                           float angle,
                           SDL_Color col)
{
    SDL_SetTextureColorMod(&texture, col.r, col.g, col.b);
    SDL_RenderCopyExF(m_renderer,
                      &texture,
                      nullptr,
                      &rect,
                      angle,
                      nullptr,
                      flip);
}

void Renderer::clear()
{
    SDL_SetRenderDrawColor(m_renderer, m_clearCol.r, m_clearCol.g, m_clearCol.b, m_clearCol.a);
    SDL_RenderClear(m_renderer);
}

void Renderer::resize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    m_wasResized = true;
}

void Renderer::checkResize()
{
    if (!m_window.checkResize())
    {
        m_wasResized = false;
        return;
    }

    resize(m_window.getWidth(), m_window.getHeight());
}