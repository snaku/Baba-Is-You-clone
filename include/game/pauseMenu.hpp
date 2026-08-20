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
        static_assert(std::is_invocable_v<Fn&, Button&>, "PauseMenu::setExitButtonCallback");

        setButtonCallback(m_menu.findButton("exit"), std::forward<Fn>(pressedCallback));
    }
};