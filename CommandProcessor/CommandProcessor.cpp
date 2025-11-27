/**
 * @file CommandProcessor.cpp
 * @author Gorden Quach 40263250
 * @date November 6th, 2025
 */
#include "CommandProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <algorithm>

namespace
{
    std::string trim(const std::string &s)
    {
        const auto first = s.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";
        const auto last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, last - first + 1);
    }

    std::vector<std::string> splitCommaList(const std::string &token)
    {
        std::vector<std::string> out;
        std::string current;
        std::stringstream ss(token);
        while (std::getline(ss, current, ','))
        {
            current = trim(current);
            if (!current.empty())
                out.push_back(current);
        }
        return out;
    }
}

// ==== COMMAND CLASS ====
/**
 * @brief default constructor
 */
Command::Command() : commandStr_(new std::string()),
                     effect_(new std::string())
{
}

/**
 * @param cmd
 * @brief constructor
 */
Command::Command(const std::string &cmd) : commandStr_(new std::string(cmd)),
                                           effect_(new std::string())
{
}

/**
 * @param c
 * @brief (shallow) copy constructor
 */
Command::Command(const Command &c) : commandStr_(nullptr),
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

Command &Command::operator=(const Command &c)
{
    std::cout << "[Command] Copy assign constructor call." << std::endl;
    if (this != &c)
    {
        destroy();
        copyFrom(c);
    }
    return *this;
}
/**
 * @param c
 * @brief soft copies the pointers of passed command to the current Command object
 */
void Command::copyFrom(const Command &c)
{
    commandStr_ = new std::string(*c.commandStr_);
    effect_ = new std::string(*c.effect_);
}
/**
 * @brief support function for destructor
 */
void Command::destroy()
{
    delete commandStr_;
    commandStr_ = nullptr;
    delete effect_;
    effect_ = nullptr;
}
/**
 * @brief destructor
 */
Command::~Command()
{
    destroy();
}

std::string Command::stringToLog()
{
    std::ostringstream oss;

    oss << "New command saved with effect: " << *this;
    ;

    return oss.str();
}

/**
 * @brief accessor for effect
 */
const std::string &Command::getEffect() const
{
    return *effect_;
}
/**
 * @brief accessor for commandStr
 */
const std::string &Command::getCommand() const
{
    return *commandStr_;
}
/**
 * @param effect
 * @brief setter for the effect attribute of the Command obj
 */
void Command::saveEffect(const std::string &effect)
{
    if (!effect_)
        effect_ = new std::string();
    *effect_ = effect;
    notify(this);
}

/**
 * @param os, cmd
 * @brief output stream operator (friend function)
 */
std::ostream &operator<<(std::ostream &os, const Command &cmd)
{
    os << "Command {commandStr = " << *cmd.commandStr_ << ", Effect = " << *cmd.effect_ << "} " << std::endl;
    return os;
}

// ==== COMMAND PROCESSOR CLASS ====
/**
 * @brief default constructor
 */
CommandProcessor::CommandProcessor() : commands_(new std::vector<Command *>())
{
    // std::cout << "[CmdProcessor] default constructor called."<< std::endl;
}

/**
 * @param cp
 * @brief (shallow) copy constructor
 */
CommandProcessor::CommandProcessor(const CommandProcessor &cp) : commands_(nullptr)
{
    std::cout << "[CmdProcessor] copy constructor called";
    if (cp.commands_)
    {
        commands_ = new std::vector<Command *>(*cp.commands_);
    }
    else
    {
        commands_ = new std::vector<Command *>();
    }
}

/**
 * @param cp
 * @brief copy-assign constructor
 * @return reference to this object after copy
 */
CommandProcessor &CommandProcessor::operator=(const CommandProcessor &cp)
{
    std::cout << "[CmdProcessor] copy-assigned constructor called";

    if (this != &cp)
    {
        delete commands_;
        if (cp.commands_)
        {
            commands_ = new std::vector<Command *>(*cp.commands_);
        }
        else
        {
            commands_ = new std::vector<Command *>();
        }
    }

    return *this;
}

/**
 * @brief destructor
 */
CommandProcessor::~CommandProcessor()
{
    for (auto *c : *commands_)
        delete c;
    delete commands_;
    commands_ = nullptr;
}

/**
 * @param os, cp
 * @brief ouput stream, (friend function)
 * @return ostream with the object attributes
 */
std::ostream &operator<<(std::ostream &os, const CommandProcessor &cp)
{
    const size_t n = (cp.commands_ ? cp.commands_->size() : 0);
    os << "CommandProcessor {Commands size = " << n << "}";
    if (cp.commands_ && !cp.commands_->empty())
    {
        os << " [\n";
        for (const Command *c : *cp.commands_)
        {
            if (c)
                os << "  " << *c; // uses Command::operator<<
        }
        os << "]";
    }
    os << "\n";
    return os;
}

std::string CommandProcessor::stringToLog()
{
    std::ostringstream oss;

    oss << "New command saved in game processor: " << *this;

    return oss.str();
}

/**
 * @brief records the command sent by the user
 * @return string command entered by user
 */
std::string CommandProcessor::readCommand()
{
    std::cout << "Enter command: ";
    std::string command;
    std::getline(std::cin, command);
    return command;
}

/**
 * @param cmd Command
 * @brief pushes the command
 */
void CommandProcessor::saveCommand(Command *cmd)
{
    commands_->push_back(cmd);
    notify(this);
}

/**
 * @return the command object owned by processor
 */
Command *CommandProcessor::getCommand(State state)
{
    std::string reading = readCommand();

    // check if empty
    if (reading.empty())
    {
        return nullptr;
    }

    // if there is input:
    Command *cmd = new Command(reading);

    // validate for the GameEngine
    bool ok = validate(cmd, state);
    if (!ok)
    {
        std::cout << "Invalid Command.\n";
    }
    saveCommand(cmd);
    return cmd;
}

/**
 * @brief Returns a copy of the current command history.
 * Each element is a pointer to a Command owned by the CommandProcessor.
 */
std::vector<Command *> CommandProcessor::getHistory() const
{
    std::vector<Command *> history;
    history.reserve(commands_->size()); // reserveing space for efficiency

    for (Command *cmd : *commands_)
    {
        history.push_back(cmd);
    }
    return history; // returns a shallow copy just to get the history
}

/**
 * @brief returns true if command is allowed in provided state and will save its effect
 * @return boolean
 * @param cmd, state
 */
bool CommandProcessor::validate(Command *cmd, State state)
{
    if (!cmd)
        return false;

    std::string s = cmd->getCommand();

    // check for command prefix for the commands that take a file.
    auto starts_with = [&](const std::string &prefix)
    {
        return s.rfind(prefix, 0) == 0; // prefix at position 0
    };

    // handle quit in all state
    if (s == "quit")
    {
        cmd->saveEffect("Program terminated");
        return true;
    }

    switch (state)
    {
    case State::START:

        //[Tournament] added new commands check
        if (starts_with("tournament"))
        {
            TournamentParams params;
            std::string error;
            if (parseTournamentCommand(s, params, error))
            {
                tournamentParams_ = params; // store for GameEngine
                cmd->saveEffect("Tournament Mode");
                return true;
            }
            else
            {
                std::cout << "Invalid tournament command: " << error << "\n";
                cmd->saveEffect("Invalid Tournament");
                return false;
            }
        }

        // Valid: loadmap <name>
        if (starts_with("loadmap"))
        {
            cmd->saveEffect("Map Loaded");
            return true;
        }

        if (s == "quit")
        {
            cmd->saveEffect("Program Terminated");
            return true;
        }
        break;

    case State::MAPLOADED:
        // Valid: validatemap, loadmap
        if (starts_with("loadmap"))
        {
            cmd->saveEffect("Map Reloaded");
            return true;
        }
        if (s == "validatemap")
        {
            cmd->saveEffect("Map Validated");
            return true;
        }
        if (s == "quit")
        {
            cmd->saveEffect("Program Terminated");
            return true;
        }
        break;

    case State::MAPVALIDATED:
        // Valid: addplayer <name>
        if (starts_with("addplayer"))
        {
            cmd->saveEffect("Player Added");
            return true;
        }
        if (s == "quit")
        {
            cmd->saveEffect("Program Terminated");
            return true;
        }
        break;

    case State::PLAYERSADDED:
        // Valid: addplayer <name>, gamestart
        if (starts_with("addplayer"))
        {
            cmd->saveEffect("Player Added");
            return true;
        }
        if (s == "gamestart")
        {
            cmd->saveEffect("Game Started");
            return true;
        }
        if (s == "quit")
        {
            cmd->saveEffect("Program Terminated");
            return true;
        }
        break;

    case State::WIN:
        // Valid: replay, quit
        if (s == "replay")
        {
            cmd->saveEffect("New Game");
            return true;
        }
        if (s == "quit")
        {
            cmd->saveEffect("Program Terminated");
            return true;
        }
        break;
    case State::ASSIGNREINFORCEMENTS:
        // placeholder
        return true;
    case State::ISSUEORDERS:
        // placeholder
        return true;
    case State::EXECUTEORDERS:
        // placeholder
        return true;
    }

    return false;
}

//[Tournament] helper function
bool CommandProcessor::parseTournamentCommand(const std::string &s, TournamentParams &tp, std::string &errorMsg)
{
    std::istringstream iss(s);
    std::string word;

    // Must start with "tournament"
    if (!(iss >> word) || word != "tournament")
    {
        errorMsg = "Command must start with 'tournament'.";
        return false;
    }

    // checks for parameters
    bool hasM = false, hasP = false, hasG = false, hasD = false;
    std::string flag;

    // Allowed player strategies as per assignment
    const std::set<std::string> allowedStrategies = {"aggressive", "benevolent", "neutral", "cheater"};

    while (iss >> flag)
    {
        // check maps
        if (flag == "-M")
        {
            std::string mapsToken;
            if (!(iss >> mapsToken))
            {
                errorMsg = "Missing list of map files after -M.";
                return false;
            }
            auto maps = splitCommaList(mapsToken);
            if (maps.size() < 1 || maps.size() > 5)
            {
                errorMsg = "M must have between 1 and 5 maps.";
                return false;
            }

            // normalize the path for the maps
            for (auto &m : maps)
            {
                // if it doesn't already start with "Resources/", will add it
                if (m.rfind("Resources/", 0) != 0)
                {
                    m = "Resources/" + m;
                }
            }

            tp.mapFiles = std::move(maps);
            hasM = true;
        }
        // check players
        else if (flag == "-P")
        {
            std::string stratToken;
            if (!(iss >> stratToken))
            {
                errorMsg = "Missing list of strategies after -P.";
                return false;
            }
            auto strategies = splitCommaList(stratToken);
            if (strategies.size() < 2 || strategies.size() > 4)
            {
                errorMsg = "P must have between 2 and 4 strategies.";
                return false;
            }

            // Optional: check they are among the known strategy names
            for (const auto &s : strategies)
            {
                if (!allowedStrategies.count(s))
                {
                    errorMsg = "Unknown player strategy: " + s;
                    return false;
                }
            }

            tp.playerStrategies = std::move(strategies);
            hasP = true;
        }
        // check number of games
        else if (flag == "-G")
        {
            int g;
            if (!(iss >> g))
            {
                errorMsg = "G must be an integer.";
                return false;
            }
            if (g < 1 || g > 5)
            {
                errorMsg = "G must be between 1 and 5.";
                return false;
            }
            tp.numGames = g;
            hasG = true;
        }

        // check # of rounds
        else if (flag == "-D")
        {
            int d;
            if (!(iss >> d))
            {
                errorMsg = "D must be an integer.";
                return false;
            }
            if (d < 10 || d > 50)
            {
                errorMsg = "D must be between 10 and 50.";
                return false;
            }
            tp.maxTurns = d;
            hasD = true;
        }
        else
        {
            errorMsg = "Unknown flag in tournament command: " + flag;
            return false;
        }
    }

    // if any of the checks are false return error message
    if (!hasM || !hasP || !hasG || !hasD)
    {
        errorMsg = "Tournament command must contain -M, -P, -G and -D.";
        return false;
    }

    return true;
}

// =========== FILE LINE READER CLASS ==============

/**
 * @brief default constructor for FLR
 */
FileLineReader::FileLineReader() : file_(nullptr),
                                   filename_(nullptr)
{
}

/**
 * @brief param cosntructor for FLR
 * @param filename string of filename
 */
FileLineReader::FileLineReader(const std::string &filename) : file_(new std::ifstream(filename)),
                                                              filename_(new std::string(filename))
{
    if (!file_->is_open())
    {
        std::cerr << "[FileLineReader] Failed to open: " << filename << "\n";
        delete file_;
        delete filename_;
        file_ = nullptr;
        filename_ = nullptr;
    }
}

/**
 * @brief copy constructor for FLR
 */
FileLineReader::FileLineReader(const FileLineReader &flr) : file_(nullptr),
                                                            filename_(nullptr)
{
    if (flr.filename_)
    {
        filename_ = new std::string(*flr.filename_);
        file_ = new std::ifstream(*filename_);
        if (!file_->is_open())
        {
            delete file_;
            file_ = nullptr;
        }
        else if (flr.file_ && flr.file_->is_open())
        {
            std::streampos pos = flr.file_->tellg();
            if (pos != std::streampos(-1))
                file_->seekg(pos);
        }
    }
}

/**
 * @brief copy assign constructor for FLR
 * will check if it is the same object, if it isn't, deep copies this* from flr
 */
FileLineReader &FileLineReader::operator=(const FileLineReader &flr)
{
    if (this != &flr)
    {

        if (file_)
        {
            file_->close();
            delete file_;
            file_ = nullptr;
        }

        if (filename_)
        {
            delete filename_;
            filename_ = nullptr;
        }

        if (flr.filename_)
        {
            filename_ = new std::string(*flr.filename_);
            file_ = new std::ifstream(*filename_);

            if (!file_->is_open())
            {
                delete file_;
                file_ = nullptr;
            }
            else if (flr.file_ && flr.file_->is_open())
            {
                std::streampos pos = flr.file_->tellg();
                if (pos != std::streampos(-1))
                    file_->seekg(pos);
            }
        }
    }
    return *this;
}

/**
 * @brief destructor for flr
 */
FileLineReader::~FileLineReader()
{
    if (file_)
    {
        file_->close();
        delete file_;
    }
    if (filename_)
    {
        delete filename_;
    }
    file_ = nullptr;
    filename_ = nullptr;
}

/**
 * @return string: next command line from the file or an empty string on EOF/error
 * @brief reads next command line from input file
 * returns empty string when EOF reached.
 */
std::string FileLineReader::readLineFromFile()
{

    // if no file or file open unsuccessful
    if (!file_ || !file_->is_open())
    {
        std::cout << "[FLR] Unable to open file or file DNE. \n";
        return {};
    }

    std::string line;
    if (std::getline(*file_, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); // trims \r
        return line;
    }

    std::cout << "[FLR] End of file reached or error.\n";
    return {};
}

/**
 * @return ostream with file or filename if open
 * @brief Stream insertion operator for FLR.
 */
std::ostream &operator<<(std::ostream &os, const FileLineReader &flr)
{
    os << "FileLineReader{ ";
    // check if flr has file
    if (flr.filename_)
    {
        os << "filename=\"" << *flr.filename_ << "\"";
        if (flr.file_ && flr.file_->is_open())
            os << ", status=open";
        else
            os << ", status=closed";
    }
    else
    {
        os << "filename=nullptr, status=none";
    }
    os << " }";
    return os;
}

// ===== FILE COMMAND PROCESSOR ADAPTER CLASS ======

/**
 * @brief default constructor
 *
 */
FCPAdapter::FCPAdapter() : CommandProcessor(), flr_(nullptr)
{
}

/**
 * @param filename name of command file
 * @brief parametrized constructor
 * opens given file for reading, prints error message if fails
 */
FCPAdapter::FCPAdapter(const std::string &filename) : CommandProcessor(),
                                                      flr_(new FileLineReader(filename))
{
}

/**
 * @param fcpa
 * @brief deep copy constructor
 * sets file to nullptr if none
 */
FCPAdapter::FCPAdapter(const FCPAdapter &fcpa) : CommandProcessor(fcpa),
                                                 flr_(fcpa.flr_ ? new FileLineReader(*fcpa.flr_) : nullptr)
{
}

/**
 * @param fcpa
 * @return reference to this* fcpa object
 * @brief deep copy and proper clean ups of existing flr if exists
 */
FCPAdapter &FCPAdapter::operator=(const FCPAdapter &fcpa)
{
    if (this != &fcpa)
    {
        // copy base class portion
        CommandProcessor::operator=(fcpa);
        delete flr_;
        flr_ = fcpa.flr_ ? new FileLineReader(*fcpa.flr_) : nullptr;
    }
    return *this;
}

/**
 * @brief destructor, closes file if exists and cleans up memory and pointers
 */
FCPAdapter::~FCPAdapter()
{
    delete flr_;
    flr_ = nullptr;
}

/**
 * @return string: next command line from the file or an empty string on EOF/error
 * @brief reads next command line from input file by passing file to flr
 * returns empty string when EOF reached.
 */
std::string FCPAdapter::readCommand()
{
    if (!flr_)
        return {};                   // no file provided
    return flr_->readLineFromFile(); // delegate to FileLineReader
}

/**
 * @return ostream with file or filename if open
 * @brief Stream insertion operator for FCPAdapter.
 */
std::ostream &operator<<(std::ostream &os, const FCPAdapter &fcpa)
{
    os << "FCPAdapter{ ";
    if (fcpa.flr_)
        os << *fcpa.flr_; // reuse FileLineReader printer
    else
        os << "FileLineReader=nullptr";
    os << " }";
    return os;
    return os;
}