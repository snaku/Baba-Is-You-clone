#pragma once

#include <SDL2/SDL.h>

// std
#include <string>

class Window
{
public:
    Window(const std::string& name, int width, int height);
    ~Window() noexcept;

    bool init();

    SDL_Window* getSDLWindow() const { return m_window; }
private:
    std::string m_name = "Undefined";
    int m_width = 1280;
    int m_height = 720;

    SDL_Window* m_window = nullptr;
};