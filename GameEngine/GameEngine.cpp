#include "GameEngine.h"
#include "..\orders\Order.h"
#include "..\orders\OrdersList.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>


GameEngine::GameEngine() {
    currentState = new State(State::START);
    currentMap = nullptr;
    gameDeck = new Deck(30);
    isSetupRunning = true;
    isGameRunning = false;
    currentMapName = "";
}

GameEngine::~GameEngine() {
    delete currentState;
    currentState = nullptr;
    delete currentMap;
    currentMap = nullptr;
    delete gameDeck;
    gameDeck = nullptr;
}

GameEngine::GameEngine(const GameEngine &other){
    currentState = new State(*other.currentState);
    currentMap = new Map(*other.currentMap);
    gameDeck = new Deck(*other.gameDeck);
    players = other.players;
    isSetupRunning = other.isSetupRunning;
    currentMapName = other.currentMapName;
}

void GameEngine::handleSetUp(std::string input) {
    MapLoader loader;
    Map* placeHolderMap;
    // Alternative input handler for testing start up phase
    notify(this);
    switch (*currentState)
    {
    case State::START:
            if(input == "loadmap"){
                std::cout <<"Available maps in the 'maps' directory: Resources/Canada.map, Resources/Americas_1792.map\n";
                std::cout << "Enter loading map name: ";
                std::string tempName;
                std::getline(std::cin, tempName);
                placeHolderMap = loader.loadMap(tempName);
                if(placeHolderMap != nullptr){
                    currentMapName = tempName;
                    std::cout << "Map " << tempName << " loaded successfully.\n";
                    currentMap = placeHolderMap;
                    *currentState = State::MAPLOADED;
                } else {
                    std::cout << "Failed to load map " << tempName << ".\n";
                }
            } else {
                std::cout << "Invalid command. Please enter loading map phase using 'loadmap'.\n";
            }
        break;
    case State::MAPLOADED:
            if(input == "validatemap"){
                if(currentMap->validate()){
                    std::cout << "Map Validated Successfully.\nTransferring to Players Added State.\n";
                    *currentState = State::PLAYERSADDED;
                }
            } else if(input == "loadmap") {
                std::cout <<"Available maps in the 'maps' directory: Resources/Canada.map, Resources/Americas_1792.map\n";
                std::cout << "Enter loading map name: ";
                std::string tempName;
                std::getline(std::cin, tempName);
                placeHolderMap = loader.loadMap(tempName);
                if(placeHolderMap != nullptr){
                    std::cout << "Map " << tempName << " loaded successfully.\n";
                    currentMap = placeHolderMap;
                } else {
                    std::cout << "Failed to load map " << tempName << ".\n";
                }
            } else {
                std::cout << "Invalid command. Please either load a map using 'loadmap' or validate the current map using 'validatemap'.\n";
            }
        break;
    case State::PLAYERSADDED:
            if(input == "addplayer"){
                if(players.size() == 6){
                    std::cout << "Maximum number of players (6) reached. Cannot add more players.\n";
                    break;
                }
                std::cout << "Enter player name: ";
                std::string playerName;
                std::getline(std::cin, playerName);
                std::cout << std::flush;
                Player newPlayer(playerName);
                if(std::find_if(players.begin(), players.end(), [&](const Player& p) { return p.getName() == playerName; }) != players.end()){
                    std::cout << "Player with name " << playerName << " already exists. Please choose a different name.\n";
                    break;
                }
                players.push_back(newPlayer);
                std::cout << "Player " << playerName << " added successfully.\n";
            } else if(input == "startgame"){
                if(players.size() >= 2 & players.size() <= 6){
                    std::cout << "Players added successfully.\nTransferring to Assign Reinforcements State.\n";
                    InitialPlayerAssignment();
                    *currentState = State::ASSIGNREINFORCEMENTS;
                    isSetupRunning = false;
                } else {
                    std::cout << "Not enough players to start the game. Please add more players using 'addplayer'.\n";
                }
            } else {
                std::cout << "Invalid command. Please add players using 'addplayer' or start the game using 'startgame'.\n";
            }
        break;
    default:
        break;
    }
}

void GameEngine::startUpPhase() {
    std::string input;

    while (isSetupRunning) {
        std::cout << "Current State: ";
        notify(this);
        switch (*currentState) {
            case State::START:
                std::cout << "START\n";
                std::cout << "Commands:\n 'loadmap'\n";
                break;
            case State::MAPLOADED:
                std::cout << "MAPLOADED\n" << "Current Map Loaded: " << currentMapName << "\n";
                std::cout << "Commands:\n 'loadmap'\n 'validatemap'\n";
                break;
            case State::PLAYERSADDED:
                std::cout << "PLAYERSADDED\n";
                std::cout << "Current Players: ";
                for (const auto& player : players) {
                    std::cout << player.getName() << ", ";
                }
                std::cout << "\n";
                std::cout << "Commands:\n 'addplayer'\n 'startgame'\n";
                break;
            default:
                break;
        }

        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        if(input == "quit"){
            isSetupRunning = false;
        } else {
            system("cls");
            handleSetUp(input);
        }

        if (*currentState == State::ASSIGNREINFORCEMENTS) {
            mainGameLoop();
        }
    }
}

void GameEngine::InitialPlayerAssignment() {
    // Shuffle players to randomize turn order
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(players.begin(), players.end(), g);

    // Assign initial territories to players in round-robin fashion
    std::vector<Territory*> territories = *currentMap->getAllTerritories();
    int numPlayers = players.size();
    int numTerritories = territories.size();
    int initialArmySize = 50; // Example initial armies per player
    int territoriestodistribute = floor(numTerritories / 2); //Keep half of the territories neutral and the rest be distributed randomnly among players

    std::cout << "NUMBER OF PLAYERS: " << numPlayers << "\n";
    std::cout << "NUMBER OF TERRITORIES: " << numTerritories << "\n";

    int rand = 0;

    for (int i = 0; i < territoriestodistribute; ++i) {
        rand = 0 + (std::rand() % (territories.size() - 0 + 1));
        Player& currentPlayer = players[i % numPlayers];
        Territory* currentTerritory = territories[rand];
        currentPlayer.addTerritory(currentTerritory);
        currentTerritory->setOwner(&currentPlayer);
        currentTerritory->setArmies(2);
        territories.erase(territories.begin() + rand);
    }

    // Each Player gets 2 cards to start. The order in which they receive them is from last player to first player for fairness.
    for (int i = numPlayers - 1; i >= 0; --i) {
        Player& currentPlayer = players[i];
        for (int j = 0; j < 2; ++j) {
            gameDeck->draw(*currentPlayer.getHand());
        }
    }

    //Each Player gets initial armies of size 50
    for (auto& player : players) {
        player.addReinforcements(initialArmySize);
    }

    std::cout << "ORDER OF PLAY: \n";
    for (const auto& player : players) {
        std::cout << player.getName() << ", ";
    }
    std::cout << "\n";

    printPlayerStats(-1);

    std::cout << "Initial player setup complete. Each player has been assigned territories and armies.\n\n";
}

void GameEngine::printPlayerStats(int roundnum) {
    if (isGameRunning) {
        std::cout << "END OF ROUND " << roundnum << "\n";
    }
    else {
        std::cout << "END OF INITIAL SETUP" << "\n";
    }
    
    std::cout << "========================================================================\n";
    for(int i = 0; i < players.size(); i++){
        std::cout << i << " - Player " << players[i].getName() << "\n";
        std::cout << "\tReinforcement Pool: " << players[i].getReinforcements() << "\n\n";
        std::cout << "\tTerritories (" << players[i].getTerritories()->size() << "): \n";

        auto ts = *players[i].getTerritories();
        std::sort(ts.begin(), ts.end(), [](Territory* a, Territory* b){ return a->getArmies() > b->getArmies();});
        for(auto& territory : ts) {
            std::cout << "\t\t" << territory->getName() << " (Armies: " << territory->getArmies() << "),\n";
        }

        std::cout << "\n";
        std::cout << "\t" << *(players[i].getHand()) <<"\n\n";
    }
    std::cout << "========================================================================\n\n";
}

void GameEngine::mainGameLoop() {
    auto* allTerritories = currentMap->getAllTerritories();
    int totalTerritoryNum = allTerritories->size();
    int roundNum = 1;
    std::cout << "In mainGameLoop() - start - TOTAL TERRITORIES (" << totalTerritoryNum << ")\n";
    isGameRunning = true;
    notify(this);

    int tempCount = 0;
    while (isGameRunning) {
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        printPlayerStats(roundNum++);

        bool hasWinner = true;
        Player* winner = nullptr;

        for (auto* territory : *allTerritories) {
            Player* owner = territory->getOwner();
            if (owner == nullptr) {
                hasWinner = false;
                break;
            }
            if (winner == nullptr) {
                winner = owner;
            }
            else if (owner != winner) {
                hasWinner = false;
                break;
            }
        }

        if (hasWinner && winner != nullptr) {
            *currentState = State::WIN;
            notify(this);
            std::cout << "PLAYER " << winner->getName() << " HAS WON THE GAME!";
            isGameRunning = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    std::cout << "In mainGameLoop() - end\n";
}

void GameEngine::reinforcementPhase() {
    *currentState = State::ASSIGNREINFORCEMENTS;
    std::cout << "A - REINFORCEMENT PHASE\n";

    std::vector<Continent *> allContinents = *currentMap->getAllContinents();

    for (auto& player : players) {
        int numOfTerritories = player.getTerritories()->size();
        int reinforcementCount = std::max(3, static_cast<int>(numOfTerritories / 3));
        int numOfContinents = 0;

        for (auto* continent : allContinents) {
            auto* territories = continent->getTerritories();
            if (territories->empty()) continue;

            bool isContinentOwner = true;
            for (auto* territory : *territories) {
                if (territory->getOwner() != &player) {
                    isContinentOwner = false;
                    break;
                }
            }

            if (isContinentOwner) {
                numOfContinents++;
                reinforcementCount += continent->getBonus();
            }
        }

        player.addReinforcements(reinforcementCount);

        std::cout << "\tPlayer " << player.getName() << " received " << reinforcementCount << " reinforcement armies, as owner of "
                  << numOfTerritories << " territories and " << numOfContinents << " continents.\n";
    }
}

void GameEngine::issueOrdersPhase() {
    *currentState = State::ISSUEORDERS;
    std::cout << "B - ISSUE ORDERS PHASE\n";

    for (auto& player : players) {
        player.issueOrder(gameDeck);
    }
}

void GameEngine::executeOrdersPhase() {
    *currentState = State::EXECUTEORDERS;
    std::cout << "C - EXECUTE ORDERS PHASE\n";

    bool ordersRemaining = true;
    while (ordersRemaining) {
        ordersRemaining = false;

        for (auto& player : players) {
            OrdersList* list = player.getOrdersList();
            auto* orders = list->getOrders();

            if (!orders->empty()) {
                ordersRemaining = true;

                // Get the order to execute
                Order* order = orders->front();
                int orderId = order->getId();

                // EXECUTE ORDER
                int result = order->execute();

                if (result == 0) {
                    std::cout << "\t" << player.getName() << " - Execute: " << *order << "\n";
                }
                else {
                    std::cerr << "\t" << player.getName() << " - Failed to execute: " << *order << "\n";
                }

                // Remove from player's orders list
                int removeResult = list->remove(orderId);
                if (removeResult != 0) {
                    std::cerr << "Failed to remove order ID: " << orderId;
                }

                delete order;
            }
        }
    }

    std::cout << "\tALL ORDERS EXECUTED\n\n";
}

std::string GameEngine::stringToLog() {
    return "Game Engine new state: " + GameEngine::stateToString(*currentState);
}


// Functions from A1

//Deprecated -> replaced by handleSetup(): Based on the current state, handle input and/or transition to the next state if the input is valid.
void GameEngine::handleInput(std::string input){
    switch (*currentState)          
    {   
        case State::START:
            if(input == "loadmap"){
                std::cout << "Transferring to Map Loaded State.\n";
                *currentState = State::MAPLOADED;
            } else {
                std::cout << "Invalid command. Please load a map using 'loadmap'.\n";
            }
            break;
        case State::MAPLOADED:
            if(input == "validatemap"){
                std::cout << "Map Loaded Successfully.\nTransferring to Map Validated State.\n";
                *currentState = State::MAPVALIDATED;
            } else if (input == "loadmap"){
                std::cout << "Loading Map\n";
            } else {
                std::cout << "Invalid command. Please validate the map using 'validatemap' or load a new map using 'loadmap'.\n";
            }
            break;
        case State::MAPVALIDATED:
            if(input == "addplayer"){
                std::cout << "Map validated successfully.\nTransferring to Players Added State.\n";
                *currentState = State::PLAYERSADDED;
            } else {
                std::cout << "Invalid command. Please add players using 'addplayer'.\n";
            }
            break;
        case State::PLAYERSADDED:
            if(input == "assignreinforcements"){
                std::cout << "Players added successfully.\nTransferring to Assign Reinforcements State.\n";
                *currentState = State::ASSIGNREINFORCEMENTS;
            } else if (input == "addplayer"){
                std::cout << "Adding Player.\n";
            } else {
                std::cout << "Invalid command. Please assign reinforcements using 'assignreinforcements' or add more players using 'addplayer'.\n";
            }
            break;
        case State::ASSIGNREINFORCEMENTS:
            if(input == "issueorder"){
                std::cout << "Reinforcements assigned successfully.\nTransferring to Issue Orders State.\n";
                *currentState = State::ISSUEORDERS;
            } else {
                std::cout << "Invalid command. Please issue orders using 'issueorder'.\n";
            }
            break;
        case State::ISSUEORDERS:
            if(input == "endissueorders"){
                std::cout << "Orders issued successfully.\nTransferring to Execute Orders State.\n";
                *currentState = State::EXECUTEORDERS;
            } else if(input == "issueorder"){
                std::cout << "Issuing Order.\n";
            } else {
                std::cout << "Invalid command. Please execute orders using 'endissueorders' or issue more orders using 'issueorder'.\n";
            }
            break;
        case State::EXECUTEORDERS:
            if(input == "win"){
                std::cout << "Orders executed successfully.\nTransferring to Win State.\n";
                *currentState = State::WIN;
            } else if(input == "executeorders"){
                std::cout << "Executing Orders.\n";
            } else if(input == "endexecuteorder"){
                std::cout << "Ending Execute Orders Phase.\nTransferring to Assignreinforcements.\n";
                *currentState = State::ASSIGNREINFORCEMENTS;
            } else {
                std::cout << "Invalid command. Please declare a win using 'win', execute more orders using 'executeorder', or end the execute orders phase using 'endexecuteorders'.\n";
            }
            break;
        case State::WIN:
            if(input == "replay"){
                std::cout << "Game Over. Restarting Game.\nTransferring to Start State.\n";
                *currentState = State::START;
            } else if(input == "quit"){
                std::cout << "Quitting Game. Goodbye!\n";
            } else {
                std::cout << "Invalid command. Please restart the game using 'replay' or quit using 'quit'.\n";
            }
            break;
    }
}

//Depracted -> replaced by startupPhase(): Runs the game loop using the handleInput function. Displays the current state and valid commands for that state.
void GameEngine::gameLoop() {
    
    std::string input;

    while (isSetupRunning) {
        std::cout << "Current State: ";
        notify(this);
        switch (*currentState) {
            case State::START:
                std::cout << "START\n";
                std::cout << "Commands:\n 'loadmap'\n";
                break;
            case State::MAPLOADED:
                std::cout << "MAPLOADED\n";
                std::cout << "Commands:\n 'loadmap \" filename \"'\n 'validatemap'\n";
                break;
            case State::MAPVALIDATED:
                std::cout << "MAPVALIDATED\n";
                std::cout << "Commands:\n 'addplayer'\n";
                break;
            case State::PLAYERSADDED:
                std::cout << "PLAYERSADDED\n";
                std::cout << "Commands:\n 'addplayer'\n 'assignreinforcements'\n";
                break;
            case State::ASSIGNREINFORCEMENTS:
                std::cout << "ASSIGNREINFORCEMENTS\n";
                std::cout << "Commands:\n 'issueorder'\n";
                break;
            case State::ISSUEORDERS:
                std::cout << "ISSUEORDERS\n";
                std::cout << "Commands:\n 'issueorder'\n 'endissueorders'\n";
                break;
            case State::EXECUTEORDERS:
                std::cout << "EXECUTEORDERS\n";
                std::cout << "Commands:\n 'executeorder'\n 'endexecuteorders'\n 'win'\n";
                break;
            case State::WIN:
                std::cout << "WIN\n";
                std::cout << "Commands:\n 'replay'\n 'quit'\n";
                break;
        }

        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        // Exit the game loop if the user wants to quit in the WIN state
        if (input == "quit" && *currentState == State::WIN) {
            isSetupRunning = false;
        } else {
            system("cls");
            handleInput(input);
        }
    }
}