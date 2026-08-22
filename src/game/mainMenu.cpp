#include "game/mainMenu.hpp"
#include "gui/menu.hpp"

#include "renderer/renderer.hpp"

// SDL2
#include <SDL2/SDL.h>

MainMenu::MainMenu(Renderer& renderer, MenuManager& menuMng) 
    : BaseMenu(renderer, menuMng, "main")
{
}

void MainMenu::init()
{
    setActive();

    m_menu.setBackground("assets/bg_main_menu.png", {255, 255, 255, 255});

    const SDL_FPoint continueBtnPos =
    {
        (m_renderer.getWidth() - s_btnsWidth) / 2.0f,
        (m_renderer.getHeight()  - s_btnsHeight) * 0.30f
    };

    Button& continueBtn = m_menu.addButton("continue",
                                           ButtonId::CHECK_BOX, // TODO: ButtonId::CONTINUE
                                           continueBtnPos,
                                           s_btnsWidth,
                                           s_btnsHeight,
                                           [](const Button& _)
                                           {
                                           });

    continueBtn.setPressDelay(0.1f);

    const SDL_FPoint playBtnPos =
    {
        (m_renderer.getWidth() - s_btnsWidth) / 2.0f,
        (m_renderer.getHeight()  - s_btnsHeight) * 0.45f
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
        (m_renderer.getHeight() - s_btnsHeight) * 0.60f
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