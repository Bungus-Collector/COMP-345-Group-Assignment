#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    void handleInput(std::string input);
    void gameLoop();

private:
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

    State* currentState;
    bool isRunning;
};

#endif

