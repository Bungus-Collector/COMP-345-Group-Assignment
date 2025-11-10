/**
 * @file CommandProcessingDriver.cpp
 * @author Gorden Quach 40263250
 * @date November 6th, 2025
 */
#include "CommandProcessor.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>


/**
 * @param hist list of commands
 * @brief prints the history of commands and size of list of commands
 */
static void printHistory(const std::vector<Command*>& hist) {
    std::cout << "\n--- Command History (" << hist.size() << ") ---\n";
    for (const Command* c : hist) {
        if (c) std::cout << *c;
    }
    std::cout << "-------------------------------\n";
}

// /**
//  * @brief checks prefix for state
//  */
// static bool starts_with(const std::string& s, const std::string& prefix) {
//     return s.rfind(prefix, 0) == 0;
// }

// // /**
// //  * @brief switch state accordingly
// //  */
// // static void transitionState(const std::string& cmd, State& st) {
// //     switch (st) {
// //         case State::START:
// //             if (starts_with(cmd, "loadmap")) st = State::MAPLOADED;
// //             break;

// //         case State::MAPLOADED:
// //             if (cmd == "validatemap") st = State::MAPVALIDATED;
// //             else if (starts_with(cmd, "loadmap")) st = State::MAPLOADED;
// //             break;

// //         case State::MAPVALIDATED:
// //             if (starts_with(cmd, "addplayer")) st = State::PLAYERSADDED;
// //             break;

// //         case State::PLAYERSADDED:
// //             if (starts_with(cmd, "addplayer")) st = State::PLAYERSADDED;
// //             else if (cmd == "gamestart") st = State::ASSIGNREINFORCEMENTS;
// //             break;

// //         case State::ASSIGNREINFORCEMENTS:
// //         case State::ISSUEORDERS:
// //         case State::EXECUTEORDERS:
// //         case State::WIN:
// //             if (cmd == "replay") st = State::START;
// //             break;
// //     }
// // }



/**
 * @brief Runs the full Part 1 demo
 */
void test(int argc, char* argv[]) {
    std::unique_ptr<CommandProcessor> cp;
    bool fromFile = (argc >= 2);

    if (fromFile) {
        // 1.2.1 + 1.2.3: Adapter route (file input)
        cp = std::make_unique<FCPAdapter>(std::string(argv[1]));
        std::cout << "[Driver] Using FCPAdapter with file: " << argv[1] << "\n";

        // 1.2.8: stream operator for adapter
        if (auto* adapter = dynamic_cast<FCPAdapter*>(cp.get())){
            std::cout << *adapter << "\n";
        }
    } else {
        // 1.2.1 + 1.2.2: Console route
        cp = std::make_unique<CommandProcessor>();
        std::cout << "[Driver] Using CommandProcessor (console). Type 'quit' or EOF to exit.\n";
    }

    State current = State::START;

    // 1.3.3: getCommand(State) returns a Command* owned by the processor
    while (true) {
        Command* cmd = cp->getCommand(current);
        if (!cmd) { std::cout << "[Driver] No more commands. Exiting.\n"; break; }

        const std::string& text = cmd->getCommand();
        const std::string& eff  = cmd->getEffect();

        std::cout << "> "  << text << "\n";
        std::cout << "  -> " << eff  << "\n";

        if (eff == "Map loaded" || eff == "Map reloaded") current = State::MAPLOADED;
        else if (eff == "Map validated")                  current = State::MAPVALIDATED;
        else if (eff == "Player added")                   current = State::PLAYERSADDED;
        else if (eff == "Game started")                   current = State::ASSIGNREINFORCEMENTS;
        else if (eff == "Program terminated")             break; // quit

        // continue reading next command
    }

    // 1.2.4 + 1.2.8: display stored commands and operator<< for processor
    printHistory(cp->getHistory());
    std::cout << *cp << "\n";

    // 1.2.8: Rule of Three — copy ctor and copy assignment
    std::cout << "\n[Driver] Copy constructing CommandProcessor cpCopy from cp...\n";
    CommandProcessor cpCopy(*cp);
    std::cout << cpCopy << "\n";

    std::cout << "[Driver] Copy assigning CommandProcessor cpAssign = cpCopy...\n";
    CommandProcessor cpAssign;
    cpAssign = cpCopy;
    std::cout << cpAssign << "\n";

    std::cout << "[Driver] Done.\n";
    exit(0);
}

/**
 * @brief main method
 * @param argc 
 * @param argv 
 */
int main(int argc, char* argv[]) {
    test(argc,argv);
    return 0;
}


