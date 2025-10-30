#include <iostream>
#include "Map/MapDriver.cpp"
#include "Player/PlayerDriver.cpp"
#include "orders/OrdersDriver.cpp"
#include "Cards/CardsDriver.cpp"
#include "GameEngine/GameEngineDriver.cpp"
#include "GameLogs/LoggingObserverDriver.cpp"

using namespace std;

int main()
{
    testLoadMaps();

    testPlayers();

    testOrdersList();

    testCards();

    testGameStates();

    testLoggingObserver();

    return 0;
}