#include <iostream>
#include <string>
#include <random>
#include "Cards.h"

using namespace std;

// CARD CLASS
// CONSTRUCTORS
Card::Card(CardType type)   // constructor
: type(type) 
{
}

Card::Card(const Card& other)   // copy constructor
: type(other.type)
{
}

Card::~Card()   // destructor
{
}

// OPERATORS
Card& Card::operator=(const Card& other)    // assignment operator
{
    if (this != &other)
    {
        type = other.type;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Card& card)    // stream insertion operator, print card type
{
    switch (card.type)
    {
        case CardType::Bomb: os << "[Bomb]"; break;
        case CardType::Reinforcement: os << "[Reinforcement]"; break;
        case CardType::Blockade: os << "[Blockade]"; break;
        case CardType::Airlift: os << "[Airlift]"; break;
        case CardType::Diplomacy: os << "[Diplomacy]"; break;
    }
    return os;
}

// METHODS
CardType Card::getType() const
{
    return type;
}

void Card::play(Hand& hand, Deck& deck)
{
    // finds this card in the hand, then moves it back to deck
    for (auto card = hand.cards.begin(); card != hand.cards.end(); ++card)
    {
        if (card->get() == this)
        {
            auto removedCard = std::move(*card);
            hand.cards.erase(card);

            deck.cards.push_back(std::move(removedCard));
            return;
        }
    }
}


// DECK CLASS
// CONSTRUCTORS
Deck::Deck(int size)    // constructor
{
    if (size < 0) size = 0;
    int cardsPerType = size / 5;
    for (int i = 0; i < cardsPerType; ++i)
    {
        cards.push_back(std::make_unique<Card>(CardType::Bomb));
        cards.push_back(std::make_unique<Card>(CardType::Reinforcement));
        cards.push_back(std::make_unique<Card>(CardType::Blockade));
        cards.push_back(std::make_unique<Card>(CardType::Airlift));
        cards.push_back(std::make_unique<Card>(CardType::Diplomacy));
    }

    int remaining = size % 5;
    for (int i = 0; i < remaining; i++)
    {
        cards.push_back(std::make_unique<Card>(static_cast<CardType>(i)));
    }
}

Deck::Deck(const Deck& other)   // copy constructor
{
    for (const auto& card : other.cards)
    {
        cards.push_back(std::make_unique<Card>(*card));
    }
}

Deck::~Deck()   // destructor
{
}

// OPERATORS
Deck& Deck::operator=(const Deck& other)    // assignment operator
{
    if (this != &other) 
    {
        cards.clear();
        for (const auto& card : other.cards)
        {
            cards.push_back(std::make_unique<Card>(*card));
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Deck& deck)    // stream insertion operator, prints list of cards in deck
{
    os << "Deck (" << deck.cards.size() << " cards): \n";
    if (deck.cards.empty())
    {
        os << "\tEmpty";
    }
    else
    {
        for (size_t i = 0; i < deck.cards.size(); ++i)
        {
            os << "\t" << *deck.cards[i];
            if (i < deck.cards.size() - 1)
            {
                os << ",\n";
            }
        }
    }
    return os;
}

// METHODS
void Deck::draw(Hand& hand)     // takes random card from deck and puts in given hand
{
    if (!cards.empty())
    {
        // select random card index
        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<std::size_t> distribution(0, cards.size() - 1);
        int index = distribution(generator);

        // move card from deck to hand
        auto removedCard = std::move(cards[index]);
        cards.erase(cards.begin() + index);
        hand.cards.push_back(std::move(removedCard));
    }
}

// HAND CLASS
// CONSTRUCTORS
Hand::Hand()    // constructor
{
}

Hand::Hand(const Hand& other)   // copy constructor
{
    for (const auto& card : other.cards)
    {
        cards.push_back(std::make_unique<Card>(*card));
    }
}

Hand::~Hand()   // destructor
{
}

// OPERATORS
Hand& Hand::operator=(const Hand& other)    // assignment operator
{
    if (this != &other)
    {
        cards.clear();
        for (const auto& card : other.cards)
        {
            cards.push_back(std::make_unique<Card>(*card));
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Hand& hand)    // stream insertion operator, prints list of cards in hand
{
    os << "Hand (" << hand.cards.size() << " cards):\n";
    if (hand.cards.empty()) 
    {
        os << "\tEmpty";
    }
    else
    {
        for (size_t i = 0; i < hand.cards.size(); ++i)
        {
            os << "\t" << *hand.cards[i];
            if (i < hand.cards.size() - 1)
            {
                os << ",\n";
            }
        }
    }
    return os;
}

// METHODS
const std::vector<std::unique_ptr<Card>>& Hand::getCards() const
{
    return cards;
}
