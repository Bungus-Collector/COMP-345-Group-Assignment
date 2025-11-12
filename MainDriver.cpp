#include <iostream>
#include "Map/MapDriver.cpp"
#include "Player/PlayerDriver.cpp"
#include "orders/OrdersDriver.cpp"
#include "Cards/CardsDriver.cpp"
#include "GameEngine/GameEngineDriver.cpp"
#include "GameLogs/LoggingObserverDriver.cpp"
#include "CommandProcessor/CommandProcessingDriver.cpp"
#include "GameLogs/LoggingObserverDriver.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    testCommandProcessor(argc, argv);

    //testGameStartupPhase();

    // testMainGameLoop() <-- coverd in testCommadnProcessor()

    testOrderExecution();

    testLoggingObserver();

    return 0;
}