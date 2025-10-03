#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    void handleInput(std::string input); //Will handle input and change states accordingly
    void gameLoop(); //Runs the game loop using the handleInput function
    void testGameStates(); //Function to test all game states

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

    State* currentState; // Pointer to the current state
    bool isRunning; // Flag to control the game loop
};

#endif

