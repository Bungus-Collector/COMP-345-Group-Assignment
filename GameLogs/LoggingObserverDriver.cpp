#include <iostream>
#include "LoggingObserver.h"
// You must include the headers for your game classes here
#include "../GameEngine/GameEngine.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"
#include "../CommandProcessor/CommandProcessor.h"

// --- DRIVER FUNCTION ---
void testLoggingObserver() {
    std::cout << "--- Starting Logging Observer Test ---" << std::endl;

    LogObserver& logObserver = LogObserver::getInstance();

    GameEngine* engine = new GameEngine();
    OrdersList* ordersList = new OrdersList();
    Player* player = new Player("John Doe");
    Territory* territory = new Territory(1, "test", nullptr, 12, player);
    Order* order = new Deploy(11, player, 11, territory);
    Command* command = new Command("big command");
    CommandProcessor* commandProcessor = new CommandProcessor();
    State state = State::EXECUTEORDERS;

    engine->attach(&logObserver);
    ordersList->attach(&logObserver);
    order->attach(&logObserver);
    command->attach(&logObserver);
    commandProcessor->attach(&logObserver);

    std::cout << "\nTriggering GameEngine state change..." << std::endl;
    engine->gameLoop();

    std::system("cls");
    std::system("clear");

    std::cout << "\nTriggering a command save..." << std::endl;
    command->saveEffect("magic effect");

    std::cout << "\nTriggering a commandProcessor save..." << std::endl;
    commandProcessor->getCommand(state);

    std::cout << "\nTriggering adding an order to an OrderList..." << std::endl;
    ordersList->add(order);

    std::cout << "\nTriggering an order execution..." << std::endl;
    order->execute();

    std::cout << "\n--- Logging Observer Test Finished ---" << std::endl;
    std::cout << "Check the 'gamelog.txt' file for the output." << std::endl;

    delete engine;
    delete ordersList;
}

// int main()
// {
//     testLoggingObserver();

//     return 0;
// }