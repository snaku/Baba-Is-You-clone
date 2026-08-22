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
    virtual ~BaseMenu();

    virtual void init() = 0;
    virtual void update(const Input& input);
    virtual void draw();

protected:
    void setActive();
    void setInactive();

    template<typename Fn>
    void setButtonCallback(Button* btn, Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_r_v<void, Fn&, Button&>, "BaseMenu::setButtonCallback");

        if (btn == nullptr)
        {
            return;
        }

        btn->setPressedCallback(std::forward<Fn>(pressedCallback));
    }

    template<typename Fn>
    void setButtonBlockCallback(Button* btn, Fn&& blockCallback)
    {
        static_assert(std::is_invocable_r_v<bool, Fn&>, "BaseMenu::setButtonBlockCallback");

        if (btn == nullptr)
        {
            return;
        }

        btn->setBlockCallback(std::forward<Fn>(blockCallback));
    }

    Renderer& m_renderer;
    Menu& m_menu;

private:
    MenuManager& m_menuMng;
    std::string m_name;
};