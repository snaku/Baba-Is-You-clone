#pragma once

#include "input/input.hpp"

// std
#include <string>
#include <memory>

class Window;
class Renderer;
class TextureManager;
class Level;
class Fade;

enum class GameState
{
    IDLE,
    MAIN_MENU,
    PLAYING,
    PAUSE
};

class Game
{
public:
    Game(std::string_view windowName, int windowWidth, int windowHeight);
    ~Game() noexcept;

    bool start();
    bool update();

private:
    bool updateStateIdle();
    bool updateStateMainMenu();
    bool updateStatePlaying();
    bool updateStatePause();

    GameState m_state = GameState::IDLE;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<TextureManager> m_textureMng;
    Input m_input;
    std::unique_ptr<Fade> m_fade;

    std::unique_ptr<Level> m_level;

    bool m_isRunning = false;
};