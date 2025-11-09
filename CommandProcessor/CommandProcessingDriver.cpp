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
    Win,
    AssignReinforcement
};

/**
 * @return lowercase copy of input string
 */
static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),[](unsigned char c){ return std::tolower(c); });
    return s;
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

        if (!valid && state == State::End) break; // safety
    }

    std::cout << "[Driver] Exiting.\n";
    return 0;
}


