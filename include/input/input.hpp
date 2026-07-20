#pragma once

#include <SDL2/SDL.h>

class Input
{
public:
    void update();

    bool isKeyDown(SDL_Scancode key) const { return m_keyboardState[key]; };

    bool quitRequested() const { return m_quitRequested; }

private:
    bool m_quitRequested = false;
    const Uint8* m_keyboardState = nullptr;
};