#include <iostream>
#include "GameEngine.h"

void testGameStates()
{

    GameEngine *game = new GameEngine();

    game->gameLoop();

    delete game;

    game = nullptr;
}

// int main(int argc, char const *argv[])
// {
//     testGameStates();
//     return 0;
// }
