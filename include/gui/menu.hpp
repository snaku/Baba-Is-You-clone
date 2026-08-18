#pragma once

#include "gui/button.hpp"

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
            std::println("Button '{}' already exist.", name);
            return *it->second;
        }

        auto btn = std::make_unique<Button>(m_renderer, m_textureMng, id, pos, width, height);
        btn->setPressedCallback(std::forward<Fn>(pressedCallback));

        Button& ref = *btn;

        m_buttons.emplace(name, std::move(btn));

        return ref;
    }

private:
    Renderer& m_renderer;
    TextureManager& m_textureMng;

    std::unordered_map<std::string, std::unique_ptr<Button>> m_buttons;
};