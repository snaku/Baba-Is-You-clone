#include "game/game.hpp"
#include "game/level.hpp"

#include "window/window.hpp"

#include "renderer/renderer.hpp"
#include "renderer/sprite.hpp"
#include "renderer/textureManager.hpp"
#include "renderer/fade.hpp"

#include "time/time.hpp"

#include <SDL2/SDL.h>

// std
#include <iostream>

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

    m_level = std::make_unique<Level>(*m_renderer, *m_textureMng, m_input, *m_fade);
    m_level->load();

    m_isRunning = true;

    m_state = GameState::MAIN_MENU;

    return true;
}

bool Game::updateStateIdle()
{
    return true;
}

bool Game::updateStateMainMenu()
{
    // TODO

    if (m_input.isKeyDown(SDL_SCANCODE_RETURN))
    {
        m_state = GameState::PLAYING;
    }

    return true;
}

bool Game::updateStatePlaying()
{
    Time::update();

    m_level->update();
    m_fade->update();

    m_renderer->setClearColor({30, 15, 8, 255});
    m_renderer->clear();

    m_level->draw();
    m_fade->draw();

    m_renderer->draw();

    return true;
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

    bool result = false;
    switch (m_state)
    {
        case GameState::IDLE:      result = updateStateIdle();     break;
        case GameState::MAIN_MENU: result = updateStateMainMenu(); break;
        case GameState::PLAYING:   result = updateStatePlaying();  break;
        case GameState::PAUSE:     result = updateStatePause();    break;
    }

    return result;
}