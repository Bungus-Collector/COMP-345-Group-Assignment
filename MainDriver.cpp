#include <iostream>
#include "Map/MapDriver.cpp"
// #include "Player/PlayerDriver.cpp"
#include "Orders/OrdersDriver.cpp"
#include "Cards/CardsDriver.cpp"
#include "GameEngine/GameEngineDriver.cpp"

using namespace std;

int main()
{
    testLoadMaps();

    // testPlayers();

    testOrdersList();

    testCards();

    testGameStates();

    return 0;
}