#pragma once

// SDL2
#include <SDL2/SDL.h>

// std
#include <array>
#include <vector>
#include <unordered_set>

class Input
{
public:
    void update();

    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyJustDown(SDL_Scancode key) const;
    bool isKeyReleased(SDL_Scancode key) const;

    bool isMouseButtonDown(uint8_t button) const;
    bool isMouseButtonReleased(uint8_t button) const;

    SDL_FPoint getMousePos() const { return m_mousePos; }
    bool quitRequested() const { return m_quitRequested; }

private:
    bool m_quitRequested = false;

    const Uint8* m_keyboardState = nullptr;
    std::unordered_set<SDL_Scancode> m_keyJustDown;
    std::unordered_set<SDL_Scancode> m_keyReleased;

    SDL_FPoint m_mousePos;

    static constexpr std::size_t s_mouseButtonCount = 6; 
    std::array<bool, s_mouseButtonCount> m_mouseDown;
    std::array<bool, s_mouseButtonCount> m_mouseReleased;
};