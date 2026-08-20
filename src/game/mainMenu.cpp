#include "game/mainMenu.hpp"
#include "gui/menuManager.hpp"

#include "renderer/renderer.hpp"

// SDL2
#include <SDL2/SDL.h>

MainMenu::MainMenu(Renderer& renderer, MenuManager& menuMng)
    : m_renderer(renderer),
      m_menuMng(menuMng),
      m_menu(m_menuMng.addMenu("main"))
{
}

void MainMenu::init()
{
    m_menuMng.setActive("main");

    const SDL_FPoint playBtnPos =
    {
        (m_renderer.getWidth() - s_btnsWidth) / 2.0f,
        (m_renderer.getHeight()  - s_btnsHeight) * 0.3f
    };

    Button& playBtn = m_menu.addButton("play",
                                       ButtonId::PLAY,
                                       playBtnPos,
                                       s_btnsWidth,
                                       s_btnsHeight,
                                       [](const Button& _)
                                       {
                                       });
    playBtn.setPressDelay(0.1f);

    const SDL_FPoint quitBtnPos =
    {
        (m_renderer.getWidth() - s_btnsWidth) / 2.0f,
        (m_renderer.getHeight() - s_btnsHeight) * 0.6f
    };

    Button& quitBtn = m_menu.addButton("quit",
                                       ButtonId::QUIT,
                                       quitBtnPos,
                                       s_btnsWidth,
                                       s_btnsHeight,
                                       [](const Button& _)
                                       {
                                       });
}

void MainMenu::update(const Input& input)
{
    m_menuMng.update(input);
}

void MainMenu::draw()
{
    m_menuMng.draw();
}