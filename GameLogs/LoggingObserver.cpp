#include "LoggingObserver.h"
#include <iostream>

//==================================================================================//
//                                  Observer Class                                  //
//==================================================================================//

Observer::~Observer() {}

//==================================================================================//
//                                  ILoggable Class                                 //
//==================================================================================//

ILoggable::~ILoggable() {}

//==================================================================================//
//                                  Subject Class                                   //
//==================================================================================//

Subject::Subject() {
    _observers = new std::list<Observer*>();
}

Subject::~Subject() {
    delete _observers;
    _observers = nullptr;
}

void Subject::attach(Observer* o) {
    _observers->push_back(o);
}

void Subject::detach(Observer* o) {
    _observers->remove(o);
}

void Subject::notify(ILoggable* subject) {
    for (Observer* observer : *_observers) {
        observer->update(subject);
    }
}

//==================================================================================//
//                                  LogObserver Class                               //
//==================================================================================//

LogObserver& LogObserver::getInstance() {
    static LogObserver instance;
    return instance;
}

LogObserver::LogObserver() {
    _logFile.open("gamelog.txt", std::ofstream::out | std::ofstream::trunc);
    if (!_logFile.is_open()) {
        std::cerr << "Error: Could not open gamelog.txt for writing." << std::endl;
    }
}

LogObserver::~LogObserver() {
    if (_logFile.is_open()) {
        _logFile.close();
    }
}

void LogObserver::update(ILoggable* subject) {
    if (_logFile.is_open() && subject != nullptr) {
        _logFile << subject->stringToLog() << std::endl;
    }
}