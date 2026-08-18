#include "game/game.hpp"
#include "game/level.hpp"
#include "game/config.hpp"

#include "window/window.hpp"

#include "renderer/renderer.hpp"
#include "renderer/sprite.hpp"
#include "renderer/textureManager.hpp"
#include "renderer/fade.hpp"

#include "time/time.hpp"

#include "gui/menu.hpp"

#include <SDL2/SDL.h>

// std
#include <iostream>
#include <functional>

Game::Game(std::string_view windowName, int windowWidth, int windowHeight)
{
    m_window = std::make_unique<Window>(std::string(windowName), windowWidth, windowHeight);
}
Game::~Game() noexcept
{
    SDL_Quit();
}

bool Game::start()
{
    if (m_isRunning)
    {
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Error while trying to initialize SDL." << std::endl;
        return false;
    }
    if (!m_window->init())
    {
        std::cerr << "Error while trying to initialize the window." << std::endl;
        return false;
    }

    m_renderer = std::make_unique<Renderer>(*m_window);

    if (!m_renderer->init())
    {
        std::cerr << "Error while trying to initialize the renderer." << std::endl;
        return false;
    }

    m_textureMng = std::make_unique<TextureManager>(*m_renderer);

    Time::init();

    m_fade = std::make_unique<Fade>(*m_renderer);

    changeState(GameState::MAIN_MENU);

    m_isRunning = true;

    return true;
}

void Game::changeState(GameState state)
{
    if (state == m_state)
    {
        return;
    }

    m_state = state;
    std::invoke(s_initStateFuncTable[std::to_underlying(state)], this);
}

void Game::initStateIdle()
{
    std::println("GAME STATE IDLE");
}

bool Game::updateStateIdle()
{
    return true;
}

void Game::initStateMainMenu()
{
    constexpr float playBtnWidth = 160.0f;
    constexpr float playBtnHeight = 80.0f;
    SDL_FPoint playBtnPos =
    {
        (GameConfig::windowWidth - playBtnWidth) / 2.0f,
        (GameConfig::windowHeight - playBtnHeight) * 0.3f
    };

    m_mainMenu = std::make_unique<Menu>(*m_renderer, *m_textureMng);

    m_mainMenu->addButton("play",
                          ButtonId::PLAY,
                          playBtnPos,
                          playBtnWidth,
                          playBtnHeight,
                          [this](const Button& btn)
                          {
                               changeState(GameState::PLAYING);
                          });
}

bool Game::updateStateMainMenu()
{
    m_mainMenu->update(m_input);

    m_renderer->setClearColor({0, 0, 0, 255});
    m_renderer->clear();

    m_mainMenu->draw();

    m_renderer->draw();

    return true;
}

void Game::initStatePlaying()
{
    m_level = std::make_unique<Level>(*m_renderer, *m_textureMng, m_input, *m_fade);
    m_level->load();
}

bool Game::updateStatePlaying()
{
    m_level->update();
    m_fade->update();

    m_renderer->setClearColor({30, 15, 8, 255});
    m_renderer->clear();

    m_level->draw();
    m_fade->draw();

    m_renderer->draw();

    return true;
}

void Game::initStatePause()
{
    // TODO
}

bool Game::updateStatePause()
{
    // TODO

    return true;
}

bool Game::update()
{
    if (!m_isRunning)
    {
        return false;
    }

    m_input.update();
    if (m_input.quitRequested())
    {
        m_isRunning = false;
        return false;
    }

    Time::update();

    return std::invoke(s_updateStateFuncTable[std::to_underlying(m_state)], this);
}