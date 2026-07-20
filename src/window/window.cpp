#include "window/window.hpp"

Window::Window(const std::string& name, int width, int height)
    : m_name(name), m_width(width), m_height(height)
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
                                0);

    if (m_window == nullptr)
    {
        return false;
    }

    return true;
}