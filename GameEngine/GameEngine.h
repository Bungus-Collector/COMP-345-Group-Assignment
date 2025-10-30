#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include "../GameLogs/LoggingObserver.h"

class GameEngine : public Subject, ILoggable {
public:
    GameEngine();
    ~GameEngine();

    GameEngine(const GameEngine &other); // Copy constructor

    void handleInput(std::string input); //Will handle input and change states accordingly
    void gameLoop(); //Runs the game loop using the handleInput function

    std::string stringToLog() override;

private:
    // Define the possible states of the game
    enum class State {
        START,
        MAPLOADED,
        MAPVALIDATED,
        PLAYERSADDED,
        ASSIGNREINFORCEMENTS,
        ISSUEORDERS,   
        EXECUTEORDERS,
        WIN
    };

    std::string stateToString(State state) {
        switch (state) {
        case State::START:                 return "START";
        case State::MAPLOADED:             return "MAP LOADED";
        case State::MAPVALIDATED:          return "MAP VALIDATED";
        case State::PLAYERSADDED:          return "PLAYERS ADDED";
        case State::ASSIGNREINFORCEMENTS:  return "ASSIGN REINFORCEMENTS";
        case State::ISSUEORDERS:           return "ISSUE ORDERS";
        case State::EXECUTEORDERS:          return "EXECUTE ORDERS";
        case State::WIN:                   return "WIN";
        default:                           return "UNKNOWN STATE";
        }
    }

    State* currentState; // Pointer to the current state
    bool isRunning; // Flag to control the game loop
};

#endif

