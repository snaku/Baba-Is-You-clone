#pragma once

#include "input/input.hpp"

// std
#include <string>
#include <memory>
#include <array>
#include <utility>

class Window;
class Renderer;
class TextureManager;
class Level;
class Fade;
class MenuManager;
class MainMenu;

enum class GameState
{
    IDLE,
    MAIN_MENU,
    PLAYING,
    PAUSE,

    MAX
};

class Game
{
public:
    Game(std::string_view windowName, int windowWidth, int windowHeight);
    ~Game() noexcept;

    bool start();
    bool update();

private:
    bool init();

    void changeState(GameState state);
    void resumeState();

    void initStateIdle();
    bool updateStateIdle();

    void initStateMainMenu();
    bool updateStateMainMenu();

    void initStatePlaying();
    bool updateStatePlaying();

    void initStatePause();
    bool updateStatePause();

    GameState m_state = GameState::IDLE;
    GameState m_oldState = GameState::IDLE;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<TextureManager> m_textureMng;
    Input m_input;
    std::unique_ptr<Fade> m_fade;

    std::unique_ptr<Level> m_level;

    std::unique_ptr<MenuManager> m_menuMng;
    std::unique_ptr<MainMenu> m_mainMenu;

    bool m_isRunning = false;

    using InitStateCallback = void (Game::*)();
    static constexpr std::array<InitStateCallback, std::to_underlying(GameState::MAX)> s_initStateFuncTable =
    {
        &Game::initStateIdle,
        &Game::initStateMainMenu,
        &Game::initStatePlaying,
        &Game::initStatePause
    };

    using UpdateStateCallback = bool (Game::*)();
    static constexpr std::array<UpdateStateCallback, std::to_underlying(GameState::MAX)> s_updateStateFuncTable =
    {
        &Game::updateStateIdle,
        &Game::updateStateMainMenu,
        &Game::updateStatePlaying,
        &Game::updateStatePause
    };
};