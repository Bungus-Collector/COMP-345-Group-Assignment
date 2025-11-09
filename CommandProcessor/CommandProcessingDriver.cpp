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
        if (auto* adapter = dynamic_cast<FCPAdapter*>(cp.get()))
            std::cout << *adapter << "\n";
    } else {
        // 1.2.1 + 1.2.2: Console route
        cp = std::make_unique<CommandProcessor>();
        std::cout << "[Driver] Using CommandProcessor (console). Type 'quit' or EOF to exit.\n";
    }

    State current = State::START;

    // 1.3.3: getCommand(State) returns a Command* owned by the processor
    while (true) {
        Command* cmd = cp->getCommand(current);
        if (!cmd) {
            std::cout << "[Driver] No more commands (EOF/invalid/empty). Exiting.\n";
            break;
        }

        std::cout << "> " << cmd->getCommand() << "\n";
        std::cout << "  -> " << cmd->getEffect() << "\n";

        // stop if user typed quit
        if (!fromFile && cmd->getCommand() == "quit")
            break;
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


