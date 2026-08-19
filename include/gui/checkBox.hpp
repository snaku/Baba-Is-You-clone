#pragma once

#include "gui/button.hpp"

#include "renderer/sprite.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <memory>
#include <functional>

class Renderer;
class TextureManager;
class Input;

class CheckBox
{
public:
    CheckBox(Renderer& renderer, TextureManager& textureMng, SDL_FPoint pos, float width, float height);

    void update(const Input& input);
    void draw();

    using Callback = std::function<void(CheckBox&)>;

    void setActivateCallback(Callback activeCallback) { m_activateCallback = std::move(activeCallback); }
    void setDeactivateCallback(Callback deactivateCallback) { m_deactivateCallback = std::move(deactivateCallback); }

private:
    Button m_button;
    Sprite m_checkMarkSprite;
    float m_checkMarkWidth;
    float m_checkMarkHeight;

    Callback m_activateCallback;
    Callback m_deactivateCallback;

    bool m_activated = false;
};