#pragma once

#include "gui/menu.hpp"

// std
#include <utility>
#include <type_traits>

class Renderer;
class Input;
class MenuManager;

class MainMenu
{
public:
    MainMenu(Renderer& renderer, MenuManager& menuMng);

    void init();
    void update(const Input& input);
    void draw();

    template<typename Fn>
    void setPlayButtonCallback(Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_v<Fn&, Button&>, "MainMenu::setPlayButtonCallback");

        setButtonCallback(m_menu.findButton("play"), std::forward<Fn>(pressedCallback));
    }

    template<typename Fn>
    void setQuitButtonCallback(Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_v<Fn&, Button&>, "MainMenu::setQuitButtonCallback");

        setButtonCallback(m_menu.findButton("quit"), std::forward<Fn>(pressedCallback));
    }

private:
    template<typename Fn>
    void setButtonCallback(Button* btn, Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_v<Fn&, Button&>, "MainMenu::setButtonCallback");

        if (btn == nullptr)
        {
            return;
        }

        btn->setPressedCallback(std::forward<Fn>(pressedCallback));
    }

    Renderer& m_renderer;
    MenuManager& m_menuMng;
    Menu& m_menu;

    static constexpr float s_btnsWidth = 160.0f;
    static constexpr float s_btnsHeight = 80.0f;
};