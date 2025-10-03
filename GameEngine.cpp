#include "GameEngine.h"
#include <iostream>
#include <cstdlib>

GameEngine::GameEngine() {
    currentState = new State(State::START);
}

GameEngine::~GameEngine() {
    delete currentState;
    currentState = nullptr;
}

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
                std::cout << "Map Loaded\n";
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
            if(input == "issueorders"){
                std::cout << "Reinforcements assigned successfully.\nTransferring to Issue Orders State.\n";
                *currentState = State::ISSUEORDERS;
            } else {
                std::cout << "Invalid command. Please issue orders using 'issueorders'.\n";
            }
            break;
        case State::ISSUEORDERS:
            if(input == "executeorders"){
                std::cout << "Orders issued successfully.\nTransferring to Execute Orders State.\n";
                *currentState = State::EXECUTEORDERS;
            } else if(input == "issueorders"){
                std::cout << "Issuing Orders.\n";
            } else {
                std::cout << "Invalid command. Please execute orders using 'executeorders' or issue more orders using 'issueorders'.\n";
            }
            break;
        case State::EXECUTEORDERS:
            if(input == "win"){
                std::cout << "Orders executed successfully.\nTransferring to Win State.\n";
                *currentState = State::WIN;
            } else if(input == "executeorders"){
                std::cout << "Executing Orders.\n";
            } else if(input == "endexecuteorders"){
                std::cout << "Ending Execute Orders Phase.\nTransferring to Assignreinforcements.\n";
                *currentState = State::ASSIGNREINFORCEMENTS;
            } else {
                std::cout << "Invalid command. Please declare a win using 'win', execute more orders using 'executeorders', or end the execute orders phase using 'endexecuteorders'.\n";
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

void GameEngine::gameLoop() {
    
std::string input;

    while (isRunning) {
        std::cout << "Current State: ";
        switch (*currentState) {
            case State::START:
                std::cout << "CURRENT STATE = START\n";
                std::cout << "Commands:\n 'loadmap'\n";
                break;
            case State::MAPLOADED:
                std::cout << "CURRENT STATE = MAPLOADED\n";
                std::cout << "Commands:\n 'loadmap'\n 'validatemap'\n";
                break;
            case State::MAPVALIDATED:
                std::cout << "CURRENT STATE = MAPVALIDATED\n";
                std::cout << "Commands:\n 'addplayer'\n";
                break;
            case State::PLAYERSADDED:
                std::cout << "CURRENT STATE = PLAYERSADDED\n";
                std::cout << "Commands:\n 'addplayer'\n 'assignreinforcements'\n";
                break;
            case State::ASSIGNREINFORCEMENTS:
                std::cout << "CURRENT STATE = ASSIGNREINFORCEMENTS\n";
                std::cout << "Commands:\n 'assignreinforcements'\n 'issueorders'\n";
                break;
            case State::ISSUEORDERS:
                std::cout << "CURRENT STATE = ISSUEORDERS\n";
                std::cout << "Commands:\n 'issueorders'\n 'executeorders'\n";
                break;
            case State::EXECUTEORDERS:
                std::cout << "CURRENT STATE = EXECUTEORDERS\n";
                std::cout << "Commands:\n 'executeorders'\n 'endexecuteorders'\n 'win'\n";
                break;
            case State::WIN:
                std::cout << "CURRENT STATE = WIN\n";
                std::cout << "Commands:\n 'replay'\n 'quit'\n";
                break;
        }

        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        if (input == "quit" && *currentState == State::WIN) {
            isRunning = false;
        } else {
            system("cls");
            handleInput(input);
        }
    }
}

