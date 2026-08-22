#pragma once

#include "game/baseMenu.hpp"

#include "gui/menu.hpp"

// std
#include <utility>
#include <type_traits>

class PauseMenu : public BaseMenu
{
public:
    PauseMenu(Renderer& renderer, MenuManager& menuMng);

    void init() override;

    template<typename Fn>
    void setExitButtonCallback(Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_r_v<void, Fn&, Button&>, "PauseMenu::setExitButtonCallback");

        setButtonCallback(m_menu.findButton("exit"), std::forward<Fn>(pressedCallback));
    }

    template<typename Fn>
    void setResumeButtonCallback(Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_r_v<void, Fn&, Button&>, "PauseMenu::setResumeButtonCallback");

        setButtonCallback(m_menu.findButton("resume"), std::forward<Fn>(pressedCallback));
    }
};