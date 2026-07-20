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
}