#include <iostream>
#include "GameEngine.h"

int main(int argc, char const *argv[])
{
    GameEngine* game = new GameEngine();
    game->testGameStates();
    delete game;
    return 0;
}
