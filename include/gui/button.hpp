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
    CHECK_BOX,
    QUIT,

    MAX
};

class Button
{
public:
    Button(Renderer& renderer, TextureManager& textureMng, ButtonId id, SDL_FPoint pos, float width, float height);
    virtual ~Button() noexcept = default;

    virtual void update(const Input& input);
    virtual void draw();

    virtual void applyOffset(SDL_FPoint delta);

    void reset();

    using PressedCallback = std::function<void(Button&)>;

    void setPressedCallback(PressedCallback callback) { m_pressedCallback = std::move(callback); }
    void setPressDelay(float delay) { m_pressDelay = delay; }

    using BlockCallback = std::function<bool()>;

    void setBlockCallback(BlockCallback callback) { m_blockCallback = std::move(callback); }

    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }

private:
    bool checkHover(SDL_FPoint mousePos) const;
    bool checkHeld(const Input& input) const;
    bool checkPress(const Input& input);
    void handleHoverAnimation();
    void handleHeldAnimation();
    void handlePress();
    bool handleBlock();
    const SpriteInfo& getSpriteInfo(ButtonId id) const;

    Sprite m_sprite;

    ButtonId m_id = ButtonId::NONE;

    SDL_FPoint m_pos;
    float m_width;
    float m_height;
    float m_originalWidth;
    float m_originalHeight;
    SDL_FPoint m_center;
    SDL_FPoint m_originalCenter;

    bool m_hovered = false;
    bool m_pressed = false;
    bool m_held = false;
    bool m_blocked = false;

    bool m_playingHoverAnim = false;

    PressedCallback m_pressedCallback;
    float m_pressDelay = 0.0f;
    float m_pressTimer = 0.0f;

    BlockCallback m_blockCallback;

    static constexpr float s_animPressRatio = 1.07f;
    static constexpr float s_animShrinkRate = 0.7f;
    static constexpr float s_animGrowthRate = 1.4f;

    static constexpr uint8_t s_hoverBrightness = 185;
    static constexpr uint8_t s_baseBrightness = 255;
    static constexpr uint8_t s_brightnessChangeRate = 255;
};