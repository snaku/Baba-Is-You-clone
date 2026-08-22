#include "game/pauseMenu.hpp"

#include "renderer/renderer.hpp"

PauseMenu::PauseMenu(Renderer& renderer, MenuManager& menuMng)
    : BaseMenu(renderer, menuMng, "pause")
{
}

void PauseMenu::init()
{
    setActive();

    m_menu.setBackground({0, 0, 0, 160});

    const SDL_FPoint resumeBtnPos =
    {
        (m_renderer.getWidth() - 120.0f) / 2.0f,
        (m_renderer.getHeight() - 65.0f) * 0.3f
    };

    Button& resumeBtn = m_menu.addButton("resume",
                                         ButtonId::CHECK_BOX, // TODO: ButtonId::RESUME
                                         resumeBtnPos,
                                         120.0f,
                                         65.0f,
                                         [](const Button& _)
                                         {
                                         });

    const SDL_FPoint exitBtnPos =
    {
        (m_renderer.getWidth() - 120.0f) / 2.0f,
        (m_renderer.getHeight() - 65.0f) * 0.5f
    };

    Button& exitBtn = m_menu.addButton("exit",
                                       ButtonId::CHECK_BOX, // TODO: ButtonId::EXIT
                                       exitBtnPos,
                                       120.0f,
                                       65.0f,
                                       [](const Button& _)
                                       {
                                       });
}