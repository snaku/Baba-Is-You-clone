#pragma once

#include "renderer/sprite.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <cstdint>
#include <functional>

class Renderer;
class TextureManager;
class Input;

enum class ButtonId
{
    NONE,

    PLAY,

    MAX
};

class Button
{
public:
    Button(Renderer& renderer, TextureManager& textureMng, ButtonId id, SDL_FPoint pos, float width, float height);

    void update(const Input& input);
    void draw();

    using PressedCallback = std::function<void(Button&)>;

    void setPressedCallback(PressedCallback callback) { m_pressedCallback = std::move(callback); }

    void setPressDelay(float delay) { m_pressDelay = delay; }

    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }

private:
    bool checkHover(SDL_FPoint mousePos) const;
    void handleHoverAnimation();
    void handleHeldAnimation();
    const SpriteInfo& getSpriteInfo(ButtonId id) const;

    Sprite m_sprite;

    ButtonId m_id = ButtonId::NONE;

    SDL_FPoint m_pos;
    float m_width;
    float m_height;
    float m_originalWidth;
    float m_originalHeight;
    SDL_FPoint m_center;

    bool m_hovered = false;
    bool m_pressed = false;
    bool m_held = false;

    PressedCallback m_pressedCallback;
    float m_pressDelay = 0.0f;
    float m_pressTimer = 0.0f;

    static constexpr float s_animPressRatio = 1.07f;
    static constexpr float s_animShrinkRate = 0.7f;
    static constexpr float s_animGrowthRate = 1.4f;

    static constexpr uint8_t s_hoverBrightness = 185;
    static constexpr uint8_t s_baseBrightness = 255;
    static constexpr uint8_t s_brightnessChangeRate = 255;
};