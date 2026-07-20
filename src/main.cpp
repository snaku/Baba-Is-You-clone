#include "game/game.hpp"
#include "game/config.hpp"
#include <SDL2/SDL.h>

int main(int argc, char** argv)
{
    Game game(GameConfig::name, GameConfig::windowWidth, GameConfig::windowHeight);

    if (!game.start())
    {
        return -1;
    }

    while (game.update());

    return 0;
}