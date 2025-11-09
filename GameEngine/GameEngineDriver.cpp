#include <iostream>
#include "GameEngine.h"

void testGameStates()
{
    GameEngine *game = new GameEngine();

    game->gameLoop();

    delete game;

    game = nullptr;
}

void testGameStartupPhase()
{
    GameEngine *game = new GameEngine();

    game->startUpPhase();

    delete game;

    game = nullptr;
}

int main(int argc, char const *argv[])
{
    testGameStartupPhase();
    return 0;
}
