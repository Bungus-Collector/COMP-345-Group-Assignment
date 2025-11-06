#include <iostream>
#include <string>
#include "Cards.h"

using namespace std;

// Test function to demonstrate card use
void testCards()
{
    Deck deck = Deck(25);
    Hand hand;

    std::cout << "===================================\n";
    std::cout << "            Cards Test             \n";
    std::cout << "===================================\n\n";

    std::cout << "0. INITIAL STATE\n\n";
    std::cout << deck << "\n\n";
    std::cout << hand << "\n\n";

    std::cout << "Calling draw() 15 times for Hand...\n\n\n";
    for (int i = 0; i < 5; i++)
    {
        deck.draw(hand);
    }
    std::cout << "1. CARDS DRAWN FOR HAND\n\n";
    std::cout << deck << "\n\n";
    std::cout << hand << "\n\n";

    std::cout << "Calling play() on 1 card from Hand...\n";
    const auto &handCards = hand.getCards();
    Card *cardToPlay = handCards[0].get();
    cardToPlay->play(hand, deck);
    std::cout << "Playing card: " << *cardToPlay << "...\n\n\n";

    std::cout << "2. ONE CARD PLAYED\n\n";
    std::cout << deck << "\n\n";
    std::cout << hand << "\n\n";

    std::cout << "Calling play() on remaining 4 cards from Hand...\n";
    while (!hand.getCards().empty())
    {
        Card *cardToPlay = hand.getCards()[0].get();
        cardToPlay->play(hand, deck);
        std::cout << "Playing card: " << *cardToPlay << "...\n";
    }

    std::cout << "\n\n3. REMAINING CARDS PLAYED\n\n";
    std::cout << deck << "\n\n";
    std::cout << hand << "\n\n";
}

// int main()
// {
//     testCards();
//     return 0;
// }