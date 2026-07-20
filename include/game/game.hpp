#pragma once

#include "input/input.hpp"

// std
#include <string>
#include <memory>

class Window;
class Renderer;
class TextureManager;
class Level;

class Game
{
public:
    Game(std::string_view windowName, int windowWidth, int windowHeight);
    ~Game() noexcept;

    bool start();
    bool update();

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<TextureManager> m_textureMng;
    Input m_input;

    std::unique_ptr<Level> m_level;

    bool m_isRunning = false;
};