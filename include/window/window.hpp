#pragma once

// SDL2
#include <SDL2/SDL.h>

// std
#include <string>

class Window
{
public:
    Window(const std::string& name, uint32_t width, uint32_t height);
    ~Window() noexcept;

    bool init();

    SDL_Window* getSDLWindow() const { return m_window; }
private:
    std::string m_name = "Undefined";
    uint32_t m_width = 1280;
    uint32_t m_height = 720;

    SDL_Window* m_window = nullptr;
};