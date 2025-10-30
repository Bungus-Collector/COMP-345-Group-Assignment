/**
 * CommandProcessor.h
 * @author Gorden Quach 402643250
 * @date October 29th, 2025
 */

 #pragma once

 #include <string>
 #include <vector>
 #include <iosfwd>
 #include <fstream>

 // forward declaration
enum class GameState;

// === COMMAND CLASS ===
class Command{
public:
    // --- Constructor and Destructor --- 
    Command(); // Default constructor
    explicit Command(const std::string cmd); // Constructor
    Command(const Command& c); 
    Command& operator=(const Command& c);
    ~Command(); // Destructor

    // Accessor
    std::string getCommand() const;
    std::string getEffect() const;

    // Mutator
    void saveEffect(const std::string& effect);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Command& c);

private:
    std::string* commandStr;
    std::string* effect;
};

// === COMMAND PROCESSOR CLASS ===
class CommandProcessor{
public:
    // --- Constructor and Destructor --- 
    CommandProcessor(); // Default constructor
    explicit CommandProcessor();// Constructor
    CommandProcessor(const CommandProcessor& cp);
    CommandProcessor& operator=(const CommandProcessor& cp);
    virtual ~CommandProcessor(); // Destructor
    
    Command* getCommand(GameState state); // validates +saves command
    std::vector<Command*> getHistory() const; // returns all saved commands that are stored in the list


    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp);

protected:
    virtual std::string readCommand();
    void saveCommand(Command* cmd);
    bool validate(Command* cmd, GameState state);

private:
    std::vector<Command*>* commands;
};


// === FileCommandProcessorAdapter ===

class FCPAdapter{
public:
    // --- Constructor and Destructor --- 
    FCPAdapter(); // Default constructor

    explicit FCPAdapter(const std::string& filename);

    FCPAdapter(const FCPAdapter& fcpa);

    FCPAdatper& operator=(const FCPAdapter& fcpa);

    ~FCPAdapter(); // Destructor

protected:
    std::string readCommand() override;

private:
    std::ifstream* file;

}
