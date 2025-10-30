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
    // --- Constructor and Destructor --- 
    Command(); // Default constructor

    explicit Command(); // Constructor

    Command(const Command& c); 

    Command& operator=(const Command& c);

    ~Command(); // Destructor
}

class CommandProcessor{
public:
    // --- Constructor and Destructor --- 
    CommandProcessor(); // Default constructor

    explicit CommandProcessor();// Constructor

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

class FCPAdapter{
public:
    // --- Constructor and Destructor --- 
    FCPAdapter(); // Default constructor

    explicit FCPAdapter();

    FCPAdapter(const FCPAdapter& fcpa);

    FCPAdatper& operator=(const FCPAdapter& fcpa);

    ~FCPAdapter(); // Destructor

    void getCommand();

    void validate();
private:
    void readCommand();
    void saveCommand();
}
