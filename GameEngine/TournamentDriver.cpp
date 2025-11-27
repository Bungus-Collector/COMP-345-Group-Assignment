#include "TournamentDriver.h"
#include "GameEngine.h"
#include <iostream>
#include <vector>

void testTournament()
{
    LogObserver* logObserver = new LogObserver();
    GameEngine *game = new GameEngine();

    game->attach(logObserver);

    game->startUpPhase();

    std::cout << "\nTournament finished.";

    delete game;
    game = nullptr;
}

// int main(int argc, char const *argv[])
// {
//     testTournament();
//     return 0;
// }