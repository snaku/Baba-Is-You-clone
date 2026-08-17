#pragma once

#include "renderer/sprite.hpp"

// SDL2
#include <SDL2/SDL.h>

class Renderer;
class textureManager;
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

    bool m_hovered = false;
    bool m_pressed = false;
    bool m_held = false;
};