#include <iostream>
#include "Player.h"
#include "PlayerStrategy.h"
#include "../Cards/cards.h"
#include "../map/map.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"

void testPlayerStrategies()
{
    // Create a game deck
    Deck *gameDeck = new Deck(20);

    // Create players
    Player *p_human = new Player("Elian (Human)");
    Player *p_aggressive = new Player("Gabe (Aggressive)");
    // Player *p_neutral = new Player("Gorden (Neutral)");

    // creating new map
    auto *c1 = new Continent("Cont1", 5);
    auto *t1 = new Territory(1, "tier1", c1, 10, p_human);
    auto *t2 = new Territory(2, "tier2", c1, 20, p_human);
    auto *t3 = new Territory(3, "tier3", c1, 30, p_human);

    // adding adjacency
    t1->addAdjacentTerritory(t2);
    t2->addAdjacentTerritory(t3);
    t3->addAdjacentTerritory(t1);

    // Assign strategies
    p_human->setStrategy(new HumanPlayerStrategy());
    p_aggressive->setStrategy(new AggressivePlayerStrategy());
    // p_neutral->setStrategy(new NeutralPlayerStrategy());

    // Assign reinforcements
    p_human->addReinforcements(5);
    p_aggressive->addReinforcements(5);
    // p_neutral->addReinforcements(5);

    // Assign territories and armies
    p_human->addTerritory(t1);
    p_human->addTerritory(t2);
    p_human->addTerritory(t3);

    t2->setOwner(p_aggressive);
    t2->setArmies(10);
    p_aggressive->addTerritory(t2);

    // t3->setOwner(p_neutral);
    // t3->setArmies(3);
    // p_neutral->addTerritory(t3);

    // t4->setOwner(p_aggressive);
    // t4->setArmies(8);
    // p_aggressive->addTerritory(t4);

    // (A) Human Player (Interactive)
    std::cout << "\n--- Issuing orders for: " << p_human->getName()
              << " (" << p_human->getStrategy()->getType() << ") ---" << std::endl;
    p_human->issueOrder(gameDeck);
    std::cout << "\nHuman player has finished issuing orders." << std::endl;

    // (B) Aggressive Player (Automatic)
    std::cout << "\n--- Issuing orders for: " << p_aggressive->getName()
              << " (" << p_aggressive->getStrategy()->getType() << ") ---" << std::endl;
    std::cout << "(This should be automatic, focusing on attacking...)" << std::endl;
    p_aggressive->issueOrder(gameDeck);

    // (C) Neutral Player (Automatic)
    // std::cout << "\n--- Issuing orders for: " << p_neutral->getName()
    //           << " (" << p_neutral->getStrategy()->getType() << ") ---" << std::endl;
    // std::cout << "(This should be automatic and do nothing...)" << std::endl;
    // p_neutral->issueOrder(gameDeck);

    delete p_human;
    delete p_aggressive;
    // delete p_neutral;

    // delete the territories we created
    delete t1;
    delete t2;
    delete t3;

    // delete the deck
    delete gameDeck;

    std::cout << "Cleanup complete. Exiting." << std::endl;
}

int main()
{
    testPlayerStrategies();

    return 0;
}