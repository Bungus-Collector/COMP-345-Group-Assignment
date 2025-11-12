#include <iostream>
#include "GameEngine.h"

void testGameStates()
{
    GameEngine *game = new GameEngine();

    game->gameLoop();

    delete game;

    game = nullptr;
}

void testMainGameLoop()
{
    GameEngine *game = new GameEngine();

    game->startUpPhase();

    delete game;

    game = nullptr;
}

// int main(int argc, char const *argv[])
// {
//     testMainGameLoop();
//     return 0;
// }
