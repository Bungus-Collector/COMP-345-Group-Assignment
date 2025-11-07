/**
 * @file CommandProcessingDriver.cpp
 * @author Gorden Quach 40263250
 * @date November 6th, 2025
 */
#include "CommandProcessor.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <memory>

/**
 * game engine states
 */
enum class State{
    Start,
    MapLoaded,
    MapValidated,
    PlayersAdded,
    GameStarted,
    End
};

/**
 * @return lowercase copy of input string
 */
static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return std::tolower(c); });
    return s;
}

/**
 * @brief Validates a command in the current state and transitions state if valid.
 * @param rawCmd  The raw command string
 * @param state   state of game engine
 * @param effect  Human-readable description of the result.
 * @return true if the command is valid in the current state; 
 * false otherwise.
 */
static bool validateAndTransition(const std::string& rawCmd, State& state, std::string& effect) {
    const std::string cmd = toLower(rawCmd);

    auto deny = [&](const char* why){
        effect = std::string("Invalid in current state (") + why + ")";
        return false;
    };

    switch (state) {
        case State::Start:
            if (cmd.rfind("loadmap", 0) == 0) { // allow "loadmap <name>"
                effect = "Map loaded";
                state = State::MapLoaded;
                return true;
            }
            if (cmd == "quit") {
                effect = "Program terminated";
                state = State::End;
                return true;
            }
            if (cmd == "replay") { // usually only at end, but harmless here
                effect = "Nothing to replay yet";
                return true;
            }
            return deny("expected loadmap or quit");

        case State::MapLoaded:
            if (cmd == "validatemap") {
                effect = "Map validated";
                state = State::MapValidated;
                return true;
            }
            if (cmd.rfind("loadmap", 0) == 0) { // reload permitted
                effect = "Map reloaded";
                state = State::MapLoaded;
                return true;
            }
            if (cmd == "quit") { effect = "Program terminated"; state = State::End; return true; }
            return deny("expected validatemap, loadmap, or quit");

        case State::MapValidated:
            if (cmd.rfind("addplayer", 0) == 0) { // allow "addplayer <name>"
                effect = "Player added";
                state = State::PlayersAdded;
                return true;
            }
            if (cmd.rfind("loadmap", 0) == 0) { effect = "Map reloaded"; state = State::MapLoaded; return true; }
            if (cmd == "quit") { effect = "Program terminated"; state = State::End; return true; }
            return deny("expected addplayer, loadmap, or quit");

        case State::PlayersAdded:
            if (cmd.rfind("addplayer", 0) == 0) { effect = "Player added"; return true; }
            if (cmd == "gamestart") { effect = "Game started"; state = State::GameStarted; return true; }
            if (cmd.rfind("loadmap", 0) == 0) { effect = "Map reloaded"; state = State::MapLoaded; return true; }
            if (cmd == "quit") { effect = "Program terminated"; state = State::End; return true; }
            return deny("expected addplayer, gamestart, loadmap, or quit");

        case State::GameStarted:
            if (cmd == "replay") { effect = "Replay requested"; state = State::Start; return true; }
            if (cmd == "quit")   { effect = "Program terminated"; state = State::End;  return true; }
            return deny("expected replay or quit");

        case State::End:
            // Nothing should be processed after End, but allow graceful exit.
            effect = "Already ended";
            return false;
    }
    effect = "Unknown state";
    return false;
}

/**
 * @brief main method
 * @param argc 
 * @param argv 
 */
int main(int argc, char* argv[]) {
    std::unique_ptr<CommandProcessor> cp;

    // Use FCPAdapter if a filename is given, else console input.
    if (argc >= 2) {
        cp = std::make_unique<FCPAdapter>(argv[1]);
        std::cout << "[Driver] Reading commands from file: " << argv[1] << "\n";
    } else {
        cp = std::make_unique<CommandProcessor>();
        std::cout << "[Driver] Reading commands from console. Type 'quit' to exit.\n";
    }

    State state = State::Start;

    while (state != State::End) {
        // getCommand call readCommand() and also create/push a Command object into the history.
        std::string cmd = cp->getCommand(static_cast<int>(state));  
        if (cmd.empty()) {
            break;
        }

        std::string effect;
        bool valid = validateAndTransition(cmd, state, effect);

        // processor attach the effect text to the lastate command object.
        cp->saveEffect(effect);

        // output result for I/O + state changes.
        std::cout << "> " << cmd << "\n  -> " << effect << "\n";

        if (!ok && state == State::End) break; // safety
    }

    std::cout << "[Driver] Exiting.\n";
    return 0;
}


