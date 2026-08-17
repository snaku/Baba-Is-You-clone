#include "input/input.hpp"

void Input::update()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            m_quitRequested = true;
        }
    }

    m_keyboardState = SDL_GetKeyboardState(nullptr);

    SDL_Point mousePos;
    Uint32 mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);

    // i = 1 since the first index is not used
    for (std::size_t i = 1; i < s_mouseButtonCount; i++)
    {
        bool downLastFrame = m_mouseDown[i];

        m_mouseDown[i] = (mouseState & SDL_BUTTON(i)) != 0;
        m_mouseReleased[i] = downLastFrame && !m_mouseDown[i];
    }

    m_mousePos = SDL_FPoint{(float)mousePos.x, (float)mousePos.y};
}

bool Input::isMouseButtonDown(uint8_t button) const
{
    if (button < SDL_BUTTON_LEFT ||
        button >= s_mouseButtonCount)
    {
        return false;
    }

    return m_mouseDown[button];
}

bool Input::isMouseButtonReleased(uint8_t button) const
{
    if (button < SDL_BUTTON_LEFT ||
        button >= s_mouseButtonCount)
    {
        return false;
    }

    return m_mouseReleased[button];
}