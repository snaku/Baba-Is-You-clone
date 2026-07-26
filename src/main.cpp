#include "game/game.hpp"
#include "game/config.hpp"

// SDL2
#include <SDL2/SDL.h>

// std
#include <cstdio>

int main(int argc, char** argv)
{
    std::setvbuf(stdout, NULL, _IONBF, 0); // TODO: remove when i'll have a real logs system

    Game game(GameConfig::name, GameConfig::windowWidth, GameConfig::windowHeight);

    if (!game.start())
    {
        return -1;
    }

    while (game.update());

    return 0;
}