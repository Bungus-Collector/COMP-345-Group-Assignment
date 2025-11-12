#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Hand;
class Deck;


// Card Types enum
enum CardType
{
    BombCard,
    ReinforcementCard,
    BlockadeCard,
    AirliftCard,
    DiplomacyCard
};


// CARD CLASS
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

    // METHODS
    CardType getType() const;
    void play(Hand& hand, Deck& deck);

private:
    CardType type;
};


// DECK CLASS
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

    // METHODS
    void draw(Hand& hand);
private:
    std::vector<std::unique_ptr<Card>> cards;

    // allows direct access to Card::type_
    friend class Card;
};


// HAND CLASS
class Hand
{
public:
    // CONSTRUCTORS
    Hand();                     // constructor
    Hand(const Hand& other);    // copy constructor
    ~Hand();                    // destructor

    // OPERATORS
    Hand& operator=(const Hand& other);     // assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Hand& hand);    // stream insertion operator

    // METHODS
    const std::vector<std::unique_ptr<Card>>& getCards() const;

private:
    std::vector<std::unique_ptr<Card>> cards;
    
    // allows direct access to Card::type_ and Deck::cards
    friend class Card;
    friend class Deck;
};

#endif