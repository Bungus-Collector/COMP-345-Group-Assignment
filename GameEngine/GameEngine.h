#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include "../GameLogs/LoggingObserver.h"
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Cards/Cards.h"
#include <vector>

class GameEngine : public Subject, ILoggable {
public:
    GameEngine();
    ~GameEngine();

    GameEngine(const GameEngine &other); // Copy constructor

    void handleSetUp(std::string input); // Alternative input handler for testing start up phase
    void startUpPhase(); //Controls the initialization before gameplay (includes loading and validating the map, and adding players).
    void InitialPlayerAssignment(); //Handles the initial assignment of players
    void printPlayerStats(int roundNum);
    void mainGameLoop(int maxTurns = -1); //Loops the game's play phase
    void removePlayer(const std::string& playerName);
    void prepareTournamentPlayers();
    void setupTournament(std::vector<std::string> mapFiles, std::vector<std::string> playerStrategies, int numOfGames, int maxTurns);
    void runTournament();
    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    void endTurn();
    void cleanupGame();
    void printTournamentResults();
    Player* getWinner();

    std::string stringToLog() override;

    // Functions from A1
    void handleInput(std::string input); //Will handle input and change states accordingly
    void gameLoop(); //Runs the game loop using the handleInput function

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
        case State::EXECUTEORDERS:         return "EXECUTE ORDERS";
        case State::WIN:                   return "WIN";
        default:                           return "UNKNOWN STATE";
        }
    }

    State* currentState; // Pointer to the current state
    bool isSetupRunning; // Flag to control the game setup
    bool isGameRunning; // Flag to control the play loop
    bool isTournament;
    std::vector<Player> players;
    Map* currentMap;
    Deck* gameDeck;
    std::string currentMapName;
    std::vector<std::string> tournamentMaps;
    std::vector<std::string> tournamentStrategies;
    int tournamentGames;
    int tournamentMaxTurns;
    std::vector<std::vector<std::string>> tournamentResults;
    std::string tournamentLog;
};

#endif
