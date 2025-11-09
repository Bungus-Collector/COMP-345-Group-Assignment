/**
 * @file CommandProcessor.cpp
 * @author Gorden Quach 40263250
 * @date November 6th, 2025
 */
#include "CommandProcessor.h"
#include <iostream>
#include <fstream>
#include "../GameEngine/GameEngine.h"
#include <sstream>

// ==== COMMAND CLASS ====
/**
 * @brief default constructor
 */
Command::Command():
    commandStr_(new std::string()),
    effect_(new std::string())
    {
        std::cout << "[Command] Calling default constructor" << std::endl;
    }


/**
 * @param cmd
 * @brief constructor
 */
Command::Command(const std::string& cmd):
    commandStr_(new std::string(cmd)),
    effect_(new std::string())
    {
        std::cout << "[Command] Calling constructor" << std::endl;
    }


/**
 * @param c
 * @brief (shallow) copy constructor
 */
Command::Command(const Command& c):
    commandStr_(nullptr),
    effect_(nullptr)
    {
        copyFrom(c);
        std::cout << "[Command] Copy contructor call." << std::endl;
    }


/**
 * @param c
 * @brief copy-assign constructor
 * @return reference to this objet
 */

 Command& Command::operator=(const Command& c){
    std::cout << "[Command] Copy assign constructor call." << std::endl;
    if (this != &c){
        destroy();
        copyFrom(c);
    }
    return *this;
}
/**
 * @param c
 * @brief soft copies the pointers of passed command to the current Command object
 */
void Command::copyFrom(const Command& c){
    commandStr_ = new std::string(*c.commandStr_);
    effect_ = new std::string(*c.effect_);
}
/**
 * @brief support function for destructor
 */
void Command::destroy(){
    delete commandStr_;
    commandStr_ = nullptr;
    delete effect_;
    effect_ = nullptr;
}
/**
 * @brief destructor
 */
Command::~Command(){
    destroy();
}
/**
 * @brief accessor for effect
 */
const std::string& Command::getEffect() const{
    return *effect_;
}
/**
 * @brief accessor for commandStr
 */
const std::string& Command::getCommand() const{
    return *commandStr_;
}
/**
 * @param effect
 * @brief setter for the effect attribute of the Command obj
 */
void Command::saveEffect(const std::string& effect){
    if(!effect_) effect_ = new std::string();
    *effect_ = effect;
}

/**
 * @param os, cmd
 * @brief output stream operator (friend function)
 */
std::ostream& operator<<(std::ostream& os, const Command& cmd){
    os << "Command {commandStr = " << cmd.commandStr_ << ", Effect = " << cmd.effect_ <<"} " << std::endl;
    return os;
}



// ==== COMMAND PROCESSOR CLASS ====
/**
 * @brief default constructor
 */
CommandProcessor::CommandProcessor():
    commands_(new std::vector<Command*>())
    {
        std::cout << "[CmdProcessor] default constructor called."<< std::endl;
    }

/**
 * @param cp
 * @brief (shallow) copy constructor
 */
CommandProcessor::CommandProcessor(const CommandProcessor& cp):
    commands_(nullptr)
    {
        std::cout << "[CmdProcessor] copy constructor called";
        if(cp.commands_){
            commands_ = new std::vector<Command*>(*cp.commands_);
        }else{
            commands_ = new std::vector<Command*>();
        }
    }

/**
 * @param cp
 * @brief copy-assign constructor
 * @return reference to this object after copy
 */
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& cp){
    std::cout <<"[CmdProcessor] copy-assigned constructor called";
    
    if (this != &cp){
        delete commands_;
        if(cp.commands_){
            commands_ = new std::vector<Command*>(*cp.commands_);
        } else {
            commands_ = new std::vector<Command*>();
        }
    }

    return *this;    
}

/**
 * @brief destructor
 */
CommandProcessor::~CommandProcessor(){
    for (auto* c: *commands_) delete c;
    delete commands_;
    commands_ = nullptr;
}

/**
 * @param os, cp
 * @brief ouput stream, (friend function)
 * @return ostream with the object attributes
 */
std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp){
    os << "CommandProcessor {Commands = " << cp.commands_ << "} \n";
    return os;
}

/**
 * @brief records the command sent by the user
 * @return string command entered by user
 */
std::string CommandProcessor::readCommand() {
    std::cout << "Enter command: ";
    std::string command;
    std::getline(std::cin, command);
    return command;
}

/**
 * @param cmd Command 
 * @brief pushes the command 
 */
void CommandProcessor::saveCommand(Command* cmd) {
    commands_->push_back(cmd);
}

/**
 * @return the command object owned by processor
 */
Command* CommandProcessor::getCommand(State state){
    std::string reading = readCommand();

    // check if empty
    if (reading.empty()){
        return nullptr;
    }

    // if there is input:
    Command* cmd = new Command(reading);

    // validate for the GameEngine 
    if(validate(cmd,state)){
        cmd->saveEffect("Command Accepted")
    } else {
        std::cout << "Invalid Command.\n";
        return nullptr;
    }
    saveCommand(cmd);
    return cmd;
}

/**
 * @brief returns true if command text is allowed in provided state
 * @return boolean
 * @param cmd, state 
 */
bool CommandProcessor::validate(Command* cmd, State state) {
    if (!cmd) return false;

    std::string s = cmd->getCommand();

    switch (state) {
        case State::Start:
            // Valid: loadmap <name>
            return starts_with("loadmap");

        case State::MapLoaded:
            // Valid: validatemap, loadmap 
            return s == "validatemap" || starts_with("loadmap");

        case State::MapValidated:
            // Valid: addplayer <name>
            return starts_with("addplayer");

        case State::PlayersAdded:
            // Valid: addplayer <name>, gamestart
            return starts_with("addplayer") || s == "gamestart";

        case State::Win:
            // Valid: replay, quit
            return (s == "quit" || s == "replay");

        case default:
            std::cout << "[Validate Error]: State entered does not exist.\n";
            return false;
    }
    // Unknown state or command
    std::cout << "[Validate Error]: Command does not exist in Command list or State does not exist.\n";
    return false;
}

// ===== FILE COMMAND PROCESSOR ADAPTER CLASS =====
/**
 * @brief default constructor
 * 
 */
FCPAdapter::FCPAdapter():
    CommandProcessor(), file_(nullptr), filename_(nullptr){
}

/**
 * @param filename name of command file
 * @brief parametrized constructor
 * opens given file for reading, prints error message if fails
 */
FCPAdapter::FCPAdapter(const std::string& filename):
    CommandProcessor(),
    file_(new std::ifstream(filename)),
    filename_(new std::string(filename)) {
        if (!file_->is_open()){
            std::cerr << "[FCPAdapter] Failed to open file: " << filename << std::endl;
            delete file_;
            file_ = nullptr;
        }
}

/**
 * @param fcpa
 * @brief deep copy constructor, reopens the same file.
 * if unable to reopen, sets file to nullptr 
 */
FCPAdapter::FCPAdapter(const FCPAdapter& fcpa):
    CommandProcessor(fcpa), file_(nullptr), filename_(nullptr){
        if (fcpa.filename_){
            filename_ = new std::string(*fcpa.filename_);
            file_ = new std::ifstream(*filename_);

            if(file->is_open()&& fcpa.file && fcpa.file-> is_open()){
                //try to mirror stream position
                std::streampos pos = fcpa.file_->tellg();
                
                if (pos != std::streampos(-1)){
                    file_->seekg(pos);
                }
            } else if(!file_ -> is_open()){
                delete file_;
                file_ = nullptr;
            }
        }
    }
/**
 * @param fcpa
 * @return reference to this object
 * @brief deep copy and proper clean ups of existing resources.
 * closes any open file before copying new one.
 */
FCPAdapter& FCPAdapter::operator=(const FCPAdapter& fcpa){
    if(this != &fcpa){
        // copy base class portion
        CommandProcessor::operator=(fcpa);
        // clean up 
        if(file_){
            file_->close(); delete file_;
            file_ = nullptr;
        }
        
        if(filename_) {
            delete filename_;
            filename_ = nullptr;
        }
        // deep copy 
        if(fcpa.filename_){
            filename_ = new std::string(*fcpa.filename_);
            file_ = new std::ifstream(*filename_);

            if(file_-> is_open() && fcpa.file_ && fcpa.file_ -> is_open()) {
                std::streampos pos = fcpa.file_ ->tellg();
                if(pos != std::streampos(-1)){
                    file_->seekg(pos);
                }
            } else if(!file_ -> is_open()){
                delete file_;
                file_ = nullptr;
            }
        }
    }
}

/**
 * @brief destructor, closes file if exists and cleans up memory and pointers
 */
FCPAdapter::~FCPAdapter(){
    if(file_){ 
        file_->close();
        delete file_;
        file_ = nullptr;
    }

    if(filename_){
        filename_->close();
        delete filename_;
        filename_=nullptr;
    }
}

/**
 * @return string: next command line from the file or an empty string on EOF/error
 * @brief reads next command line from input file
 * returns empty string when EOF reached.
 */
std::string FCPAdapter::readCommand(){
    // If no file stream is open but we have a filename, try opening it.
    if (!file_ && filename_) {
        file_ = new std::ifstream(*filename_);
        if (!file_->is_open()) {
            std::cerr << "[FCPAdapter] Failed to (re)open file: " << *filename_ << "\n";
            delete file_;
            file_ = nullptr;
        }
    }

    // If the file still isn't open, nothing to read.
    if (!file_ || !file_->is_open()) {
        return {};
    }

    // Attempt to read a single line.
    std::string line;
    if (std::getline(*file_, line)) {
        // trailing '\r' 
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        return line; // return the next command line
    }

    // return empty to signal end-of-file
    return {};
}

/**
 * @return ostream with file or filename if open
 * @brief Stream insertion operator for FCPAdapter.
 */
std::ostream& operator<<(std::ostream& os, const FCPAdapter& adapter) {
    os << "FCPAdapter{ ";
    if (adapter.filename_) {
        os << "filename=\"" << *adapter.filename_ << "\"";
        if (adapter.file_ && adapter.file_->is_open())
            os << ", status=open";
        else
            os << ", status=closed";
    } else {
        os << "filename=nullptr, status=none";
    }
    os << " }";
    return os;
}