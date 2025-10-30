/**
 * CommandProcessor.h
 * @author Gorden Quach 402643250
 * @date October 29th, 2025
 */

 #pragma once

 #include <string>
 #include <vector>
 #include <iosfwd>

 // forward declaration
class Command{
    
}

class CommandProcessor{
public:
    // --- Constructor and Destructor --- 
    CommandProcessor(); // Default constructor

    explicit CommandProcessor();// constructor

    CommandProcessor(const CommandProcessor& cp);

    CommandProcessor& operator=(const CommandProcessor& cp);

    ~CommandProcessor(); // Destructor

    
    
    void getCommand();

    void validate();

    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& cp);
private:
    void readCommand();
    void saveCommand();
}

class FileCommandProcessorAdapter{
public:
    // --- Constructor and Destructor --- 
    FileCommandProcessorAdapter(); // Default constructor

    ~FileCommandProcessorAdapter(); // Destructor

    void getCommand();

    void validate();
private:
    void readCommand();
    void saveCommand();
}
