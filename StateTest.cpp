#include <iostream>
#include "GameEngine.h"

int main(int argc, char const *argv[])
{
    GameEngine* game = new GameEngine();
    game->gameLoop();
    delete game;
    return 0;
}
