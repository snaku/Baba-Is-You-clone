#pragma once

#include "gui/button.hpp"
#include "gui/checkBox.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <unordered_map>
#include <string>
#include <memory>
#include <type_traits>
#include <print>

class Renderer;
class TextureManager;
class Input;

class Menu
{
public:
    Menu(Renderer& renderer, TextureManager& textureMng);

    void update(const Input& input);
    void draw();

    template<typename Fn>
    Button& addButton(const std::string& name,
                      ButtonId id,
                      SDL_FPoint pos,
                      float width,
                      float height,
                      Fn&& pressedCallback)
    {
        static_assert(std::is_invocable_v<Fn&, Button&>, "Menu::addButton");

        auto it = m_buttons.find(name);
        if (it != m_buttons.end())
        {
            std::println("Button '{}' already exists.", name);
            return *it->second;
        }

        auto btn = std::make_unique<Button>(m_renderer, m_textureMng, id, pos, width, height);
        btn->setPressedCallback(std::forward<Fn>(pressedCallback));

        Button& ref = *btn;

        m_buttons.emplace(name, std::move(btn));

        return ref;
    }

    Button* findButton(const std::string& name);

    template<typename FnActivate, typename FnDeactivate>
    CheckBox& addCheckBox(const std::string& name,
                          SDL_FPoint pos,
                          float width,
                          float height,
                          FnActivate&& activateCallback,
                          FnDeactivate&& deactivateCallback)
    {
        static_assert(std::is_invocable_v<FnActivate&, CheckBox&>, "Menu::addCheckBox (ACTIVATE)");
        static_assert(std::is_invocable_v<FnDeactivate&, CheckBox&>, "Menu::addCheckBox (DEACTIVATE)");

        auto it = m_checkBoxes.find(name);
        if (it != m_checkBoxes.end())
        {
            std::println("CheckBox '{}' already exists.", name);
            return *it->second;
        }

        auto checkBox = std::make_unique<CheckBox>(m_renderer, m_textureMng, pos, width, height);
        
        checkBox->setActivateCallback(std::forward<FnActivate>(activateCallback));
        checkBox->setDeactivateCallback(std::forward<FnDeactivate>(deactivateCallback));

        CheckBox& ref = *checkBox;

        m_checkBoxes.emplace(name, std::move(checkBox));

        return ref;
    }

private:
    void resize(uint32_t windowWidth, uint32_t windowHeight);

    Renderer& m_renderer;
    TextureManager& m_textureMng;

    std::unordered_map<std::string, std::unique_ptr<Button>> m_buttons;
    std::unordered_map<std::string, std::unique_ptr<CheckBox>> m_checkBoxes;

    uint32_t m_originalWindowWidth = 0;
    uint32_t m_originalWindowHeight = 0;
};