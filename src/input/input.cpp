#include "input/input.hpp"

void Input::update()
{
    SDL_Event event;

    m_keyJustDown.clear();
    m_keyReleased.clear();

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:  m_quitRequested = true; break;

            case SDL_KEYUP: m_keyReleased.insert(event.key.keysym.scancode); break;
            case SDL_KEYDOWN:
                if (!event.key.repeat)
                {
                    m_keyJustDown.insert(event.key.keysym.scancode);
                }
                break;
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
        m_mouseJustDown[i] = !downLastFrame && m_mouseDown[i];
    }

    m_mousePos = SDL_FPoint{(float)mousePos.x, (float)mousePos.y};
}

bool Input::isKeyDown(SDL_Scancode key) const
{
    if (key >= SDL_NUM_SCANCODES)
    {
        return false;
    }

    return m_keyboardState[key];
}

bool Input::isKeyJustDown(SDL_Scancode key) const
{
    if (key >= SDL_NUM_SCANCODES)
    {
        return false;
    }

    return m_keyJustDown.contains(key);
}

bool Input::isKeyReleased(SDL_Scancode key) const
{
    if (key >= SDL_NUM_SCANCODES)
    {
        return false;
    }

    return m_keyReleased.contains(key);
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

bool Input::isMouseButtonJustDown(uint8_t button) const
{
    if (button < SDL_BUTTON_LEFT ||
        button >= s_mouseButtonCount)
    {
        return false;
    }

    return m_mouseJustDown[button];
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