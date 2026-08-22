#pragma once

#include "game/baseMenu.hpp"

#include "gui/menu.hpp"

// std
#include <utility>
#include <type_traits>

class Renderer;
class Input;
class MenuManager;

class MainMenu : public BaseMenu
{
public:
    MainMenu(Renderer& renderer, MenuManager& menuMng);

    void init() override;

    template<typename Fn>
    void setContinueButtonBlockCallback(Fn&& blockCallback)
    {
        static_assert(std::is_invocable_r_v<bool, Fn&>, "MainMenu::setContinueButtonBlockCallback");

        setButtonBlockCallback(m_menu.findButton("continue"), std::forward<Fn>(blockCallback));
    }

    template<typename Fn>
    void setPlayButtonCallback(Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_r_v<void, Fn&, Button&>, "MainMenu::setPlayButtonCallback");

        setButtonCallback(m_menu.findButton("play"), std::forward<Fn>(pressedCallback));
    }

    template<typename Fn>
    void setQuitButtonCallback(Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_r_v<void, Fn&, Button&>, "MainMenu::setQuitButtonCallback");

        setButtonCallback(m_menu.findButton("quit"), std::forward<Fn>(pressedCallback));
    }

private:
    static constexpr float s_btnsWidth = 160.0f;
    static constexpr float s_btnsHeight = 80.0f;
};