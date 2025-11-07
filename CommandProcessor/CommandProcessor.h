/**
 * @file CommandProcessor.h
 * @author Gorden Quach 402643250
 * @date October 29th, 2025
 */

 #pragma once

 #include <string>
 #include <vector>
 #include <iosfwd>
 #include <fstream>

 // forward declaration
enum class State;

// === COMMAND CLASS ===
/**
 * @class Command
 * @brief Represents a single command and its corresponding effect.
 */
class Command{
public:
    // --- Constructor and Destructor --- 
    Command(); // Default constructor
    explicit Command(const std::string cmd); // Constructor
    Command(const Command& c); 
    Command& operator=(const Command& c);
    ~Command(); // Destructor

    // Accessor
    const std::string& getCommand() const;
    const std::string& getEffect() const;

    // Mutator
    void saveEffect(const std::string& effect);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Command& c);

private:
    std::string* commandStr_;
    std::string* effect_;

    // Additional methods for copy and destroy
    void copyFrom(const Command& c);
    void destroy();
};

// === COMMAND PROCESSOR CLASS ===
/**
 * @class CommandProcessor
 * @brief Handles command input and stores all executed commands.
 * @details Implements the basic Command pattern by maintaining a list of Command objects entered by the user or read from a file.
 */
class CommandProcessor{
public:
    // --- Constructor and Destructor --- 
    CommandProcessor(); // Default constructor
    CommandProcessor(const CommandProcessor& cp);
    CommandProcessor& operator=(const CommandProcessor& cp);
    virtual ~CommandProcessor(); // Destructor
    
    Command* getCommand(State state); // validates +saves command
    std::vector<Command*> getHistory() const; // returns all saved commands that are stored in the list


    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp);

protected:
    virtual std::string readCommand();
    void saveCommand(Command* cmd);
    bool validate(Command* cmd, State state);

private:
    std::vector<Command*>* commands_{};
};


// === FileCommandProcessorAdapter ===
/**
 * @class FCPAdapter
 * @brief Adapter that allows CommandProcessor to read commands from a file.
 * @details Implements the Adapter design pattern by overriding readCommand() to pull commands sequentially from a text file instead of console input.
 */
class FCPAdapter : public CommandProcessor {
public:
    // --- Constructor and Destructor --- 
    FCPAdapter(); // Default constructor

    explicit FCPAdapter(const std::string& filename);

    FCPAdapter(const FCPAdapter& fcpa);

    FCPAdapter& operator=(const FCPAdapter& fcpa);

    ~FCPAdapter() override; // Destructor

protected:
    std::string readCommand() override;

private:
    std::ifstream* file_{};
    std::string* filename_{};
};
