#include <iostream>
// #include "Map/MapDriver.h"
// #include "orders/OrdersDriver.h"
// #include "GameEngine/GameEngineDriver.h"
// #include "GameLogs/LoggingObserverDriver.h"
// #include "CommandProcessor/CommandProcessingDriver.h"
#include "Player/PlayerStrategiesDriver.h"
#include "GameEngine/TournamentDriver.h"

using namespace std;

int main(int argc, char *argv[])
{
    // std::cout << "[1] - testCommandProcessor()\n\n";
    // testCommandProcessor(argc, argv);

    // testStartupPhase() --> part of testMainGameLoop()

    // std::cout << "[2] - testStartupPhase()\n\n";
    // testMainGameLoop();

    // std::cout << "[4] - testOrderExecution()\n\n";
    // testOrderExecution();

    // std::cout << "[5] - testLoggingObserver()\n\n";
    // testLoggingObserver();

    std::cout << "=============================== Testing player strategies ===============================" << endl;
    testPlayerStrategies();

    std::cout << "=============================== Testing tournament mode ===============================" << endl;
    testTournament();

    return 0;
}