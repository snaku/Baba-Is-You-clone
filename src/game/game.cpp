#include "game/game.hpp"
#include "game/level.hpp"
#include "game/mainMenu.hpp"
#include "game/pauseMenu.hpp"
#include "game/config.hpp"
#include "game/saveSystem.hpp"

#include "window/window.hpp"

#include "renderer/renderer.hpp"
#include "renderer/sprite.hpp"
#include "renderer/textureManager.hpp"
#include "renderer/fade.hpp"

#include "time/time.hpp"

#include "gui/menuManager.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <iostream>
#include <functional>

// TODO: refactor

Game::Game(std::string_view windowName, int windowWidth, int windowHeight)
{
    m_window = std::make_unique<Window>(std::string(windowName), windowWidth, windowHeight);
}
Game::~Game() noexcept
{
}

bool Game::start()
{
    if (m_isRunning)
    {
        return false;
    }

    if (!init())
    {
        return false;
    }

    m_isRunning = true;

    changeState(GameState::MAIN_MENU);

    return true;
}

bool Game::init()
{
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

    Time::init();

    m_textureMng = std::make_unique<TextureManager>(*m_renderer);
    m_fade = std::make_unique<Fade>(*m_renderer);
    m_menuMng = std::make_unique<MenuManager>(*m_renderer, *m_textureMng);

    return true;
}

void Game::changeState(GameState state)
{
    if (state == m_state)
    {
        return;
    }

    m_oldState = m_state;
    m_state = state;
    std::invoke(s_initStateFuncTable[std::to_underlying(state)], this);
}

void Game::resumeState()
{
    std::swap(m_state, m_oldState);
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
    m_mainMenu = std::make_unique<MainMenu>(*m_renderer, *m_menuMng);
    m_mainMenu->init();

    m_mainMenu->setContinueButtonBlockCallback(
        []()
        {
            return !SaveSystem::hasData(); // blocks if no save data, hence i use '!'
        }
    );

    m_mainMenu->setContinueButtonCallback(
        [this](const Button& _)
        {
            m_playMode = PlayMode::CONTINUE;
            changeState(GameState::PLAYING);
        }
    );

    m_mainMenu->setPlayButtonCallback(
        [this](const Button& _)
        {
            m_playMode = PlayMode::NEWGAME;
            changeState(GameState::PLAYING);
        }
    );

    m_mainMenu->setQuitButtonCallback(
        [this](const Button& _)
        {
            m_isRunning = false;
        }
    );

    m_renderer->setClearColor({0, 0, 0, 255});
}

bool Game::updateStateMainMenu()
{
    m_mainMenu->update(m_input);

    m_renderer->clear();

    m_mainMenu->draw();

    m_renderer->draw();

    return true;
}

void Game::initStatePlaying()
{
    m_level = std::make_unique<Level>(*m_renderer, *m_textureMng, m_input, *m_fade);
    
    switch (m_playMode)
    {
        case PlayMode::NEWGAME:
            SaveSystem::save(0); // overwrites if a save already exists
            m_level->load(0);
            break;

        case PlayMode::CONTINUE:
            Save save = SaveSystem::load();
            if (save.levelId.has_value())
            {
                m_level->load(save.levelId.value());
            }
            else
            {
                changeState(GameState::IDLE);
                return;
            }
    }

    m_level->setWinCallback(
        [](Level& level)
        {
            SaveSystem::save(level.getId());
        }
    );

    m_renderer->setClearColor({30, 15, 8, 255});
}

bool Game::updateStatePlaying()
{
    if (m_input.isKeyJustDown(SDL_SCANCODE_ESCAPE))
    {
        m_pauseMenu.reset();
        changeState(GameState::PAUSE);
        return true;
    }

    if (m_renderer->wasResized())
    {
        m_level->resize(m_renderer->getWidth(), m_renderer->getHeight());
    }

    m_level->update();
    m_fade->update();

    m_renderer->clear();

    m_level->draw();
    m_fade->draw();

    m_renderer->draw();

    return true;
}

void Game::initStatePause()
{
    m_pauseMenu = std::make_unique<PauseMenu>(*m_renderer, *m_menuMng);
    m_pauseMenu->init();

    m_pauseMenu->setExitButtonCallback(
        [this](const Button& _)
        {
            m_level.reset();
            m_mainMenu.reset();
            changeState(GameState::MAIN_MENU);
        }
    );

    m_pauseMenu->setResumeButtonCallback(
        [this](const Button& _)
        {
            resumeState(); // resume GameState::PLAYING since we come from it
        }
    );
}

bool Game::updateStatePause()
{
    if (m_input.isKeyJustDown(SDL_SCANCODE_ESCAPE))
    {
        resumeState();
        return true;
    }

    if (m_renderer->wasResized())
    {
        m_level->resize(m_renderer->getWidth(), m_renderer->getHeight());
    }

    m_pauseMenu->update(m_input);

    // if exit button was pressed
    if (m_state != GameState::PAUSE)
    {
        return true;
    }

    m_renderer->clear();

    m_level->draw();
    m_pauseMenu->draw();

    m_renderer->draw();

    return true;
}

bool Game::update()
{
    if (!m_isRunning)
    {
        return false;
    }

    m_renderer->checkResize();

    m_input.update();
    if (m_input.quitRequested())
    {
        m_isRunning = false;
        return false;
    }

    Time::update();

    return std::invoke(s_updateStateFuncTable[std::to_underlying(m_state)], this);
}