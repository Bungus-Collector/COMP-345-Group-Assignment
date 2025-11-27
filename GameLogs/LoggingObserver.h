#pragma once

#include <list>
#include <string>
#include <fstream>

class ILoggable;

/**
 * @class Observer
 * @brief The interface of an observer.
 */
class Observer {
public:
    virtual ~Observer();

    /**
    * Update logs 
    *
    * @param subject the member that update the logs.
    */
    virtual void update(ILoggable* subject) = 0;
};

/**
 * @class ILoggable
 * @brief Interface for classes that are loggable.
 */
class ILoggable {
public:
    virtual ~ILoggable();

    /**
    * Creates and return string to be put in log file.
    */
    virtual std::string stringToLog() = 0;
};

/**
 * @class Subject
 * @brief The interface of an observer.
 */
class Subject {
public:
    Subject();
    virtual ~Subject();

    /**
    * Subscribe an observer to subject.
    *
    * @param observer to subscribe.
    */
    virtual void attach(Observer* observer);

    /**
    * Unsubscribe an observer to subject.
    *
    * @param observer to unsubscribe.
    */
    virtual void detach(Observer* observer);

    /**
    * Notify all subscribed observers.
    *
    * @param subject to notify.
    */
    virtual void notify(ILoggable* subject);

private:
    std::list<Observer*>* _observers;
};

/**
 * @class LogObserver
 * @brief The concrete implementation of an observer class.
 */
class LogObserver : public Observer {
public:
    static LogObserver& getInstance();

    /**
    * Update logs
    *
    * @param subject the member that update the logs.
    */
    void update(ILoggable* subject) override;

private:
    LogObserver();
    ~LogObserver();

    LogObserver(const LogObserver&) = delete;
    LogObserver& operator=(const LogObserver&) = delete;

    std::ofstream _logFile;
};