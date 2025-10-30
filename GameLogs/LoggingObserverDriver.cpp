#include <iostream>
#include "LoggingObserver.h"
// You must include the headers for your game classes here
#include "../GameEngine/GameEngine.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"
// #include "CommandProcessing.h"
// #include "Command.h"

// --- DRIVER FUNCTION ---
void testLoggingObserver() {
    std::cout << "--- Starting Logging Observer Test ---" << std::endl;

    LogObserver* logObserver = new LogObserver();

    GameEngine* engine = new GameEngine();
    OrdersList* ordersList = new OrdersList();
    Order* order = new Order(11);

    engine->attach(logObserver);
    ordersList->attach(logObserver);
    order->attach(logObserver);

    std::cout << "\nTriggering GameEngine state change..." << std::endl;
    engine->gameLoop();

    std::cout << "\nTriggering adding an order to an OrderList..." << std::endl;
    ordersList->add(order);

    std::cout << "\nTriggering an order execution..." << std::endl;
    order->execute();

    std::cout << "\n--- Logging Observer Test Finished ---" << std::endl;
    std::cout << "Check the 'gamelog.txt' file for the output." << std::endl;

    delete logObserver;
    delete engine;
    delete ordersList;
    delete order;
}