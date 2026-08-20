#include "window/window.hpp"

Window::Window(const std::string& name, uint32_t width, uint32_t height)
    : m_name(name),
      m_width(width),
      m_height(height)
{
}
Window::~Window() noexcept
{
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
    }
}

bool Window::init()
{
    m_window = SDL_CreateWindow(m_name.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_width,
                                m_height,
                                SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr)
    {
        return false;
    }

    return true;
}

bool Window::checkResize()
{
    int width;
    int height;
    SDL_GetWindowSize(m_window, &width, &height);

    if ((uint32_t)width == m_width &&
        (uint32_t)height == m_height)
    {
        return false;
    }

    m_width = width;
    m_height = height;

    return true;
}