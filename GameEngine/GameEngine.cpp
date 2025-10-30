#include "GameEngine.h"
#include <iostream>
#include <cstdlib>

GameEngine::GameEngine() {
    currentState = new State(State::START);
    isRunning = true;
}

GameEngine::~GameEngine() {
    delete currentState;
    currentState = nullptr;
}

GameEngine::GameEngine(const GameEngine &other){
    currentState = new State(*other.currentState);
    isRunning = other.isRunning;
}

//Based on the current state, handle input and/or transition to the next state if the input is valid.
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

//Runs the game loop using the handleInput function. Displays the current state and valid commands for that state.
void GameEngine::gameLoop() {
    
    std::string input;

    while (isRunning) {
        std::cout << "Current State: ";
        notify(this);
        switch (*currentState) {
            case State::START:
                std::cout << "START\n";
                std::cout << "Commands:\n 'loadmap'\n";
                break;
            case State::MAPLOADED:
                std::cout << "MAPLOADED\n";
                std::cout << "Commands:\n 'loadmap'\n 'validatemap'\n";
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
            isRunning = false;
        } else {
            system("cls");
            handleInput(input);
        }
    }
}

std::string GameEngine::stringToLog() {
    return "Game Engine new state: " + GameEngine::stateToString(*currentState);
}

