#include "TournamentDriver.h"
#include "GameEngine.h"
#include <iostream>
#include <vector>

void testTournament() {
    // TODO: add command processing for tournament

    // temporary variables
    std::vector<std::string> tournamentMaps;
    // tournamentMaps.push_back("Resources/Demo_Map.map");
    tournamentMaps.push_back("Resources/Canada.map");

    std::vector<std::string> tournamentStrategies;
    tournamentStrategies.push_back("aggressive");
    tournamentStrategies.push_back("neutral");
    tournamentStrategies.push_back("benevolent");
    tournamentStrategies.push_back("cheater");

    int tournamentGames = 3;
    int tournamentMaxTurns = 40;

    GameEngine *game = new GameEngine();

    game->setupTournament(tournamentMaps, tournamentStrategies, tournamentGames, tournamentMaxTurns);
    game->runTournament();

    std::cout << "\nTournament finished.";

    delete game;
    game = nullptr;
}

int main(int argc, char const *argv[])
{
    testTournament();
    return 0;
}