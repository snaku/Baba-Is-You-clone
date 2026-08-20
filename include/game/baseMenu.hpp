#pragma once

#include "gui/button.hpp"

// std
#include <utility>
#include <type_traits>

class Renderer;
class Input;
class MenuManager;
class Menu;

class BaseMenu
{
public:
    BaseMenu(Renderer& renderer, MenuManager& menuMng, const std::string& menuName);
    virtual ~BaseMenu() = default;

    virtual void init() = 0;
    virtual void update(const Input& input);
    virtual void draw();

protected:
    void setActive();

    template<typename Fn>
    void setButtonCallback(Button* btn, Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_v<Fn&, Button&>, "BaseMenu::setButtonCallback");

        if (btn == nullptr)
        {
            return;
        }

        btn->setPressedCallback(std::forward<Fn>(pressedCallback));
    }

    Renderer& m_renderer;
    Menu& m_menu;

private:
    MenuManager& m_menuMng;
    std::string m_name;
};