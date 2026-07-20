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

    return true;
}

bool Game::update()
{
    m_input.update();

    if (m_input.quitRequested())
    {
        m_isRunning = false;
        return false;
    }

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