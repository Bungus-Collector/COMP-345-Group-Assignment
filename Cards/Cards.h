// Author: Paul Humennyj
#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Card Types enum
enum CardType
{
    Bomb,
    Reinforcement,
    Blockade,
    Airlift,
    Diplomacy
};


// Card class
class Card
{
public:
    // CONSTRUCTORS
    Card(CardType type);        // constructor
    Card(const Card& other);    // copy constructor
    ~Card();                    // desctructor

    // OPERATORS
    Card& operator=(const Card& other);     // assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Card& card);    // stream insertion operator

    // DATA MEMBERS
    CardType type;

    // METHODS
    CardType getType() const;
    void play();
};

// Deck class
class Deck
{
public:
    // CONSTRUCTORS
    Deck(int size);             // constructor
    Deck(const Deck& other);    // copy constructor
    ~Deck();                    // destructor

    //OPERATORS
    Deck& operator=(const Deck& other);     // assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Deck& deck);    // stream insertion operator

    // DATA MEMBERS
    vector<std::unique_ptr<Card>> cards;

    // METHODS
    Card draw();
};

// Hand class
class Hand
{
public:
    // CONSTRUCTORS
    Hand();                     // constructor
    Hand(const Hand& other);    // copy constructor
    ~Hand();                    // destructor

    //OPERATORS
    Hand& operator=(const Hand& other);     // assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Hand& hand);    // stream insertion operator

    // DATA MEMBERS
    vector<std::unique_ptr<Card>> cards;
};

#endif