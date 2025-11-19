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


    // Continent for benevolent scenario
    auto *c2 = new Continent("Cont2", 5);
    auto *b1 = new Territory(4, "Benev1", c2, 2,  p_benevolent);  // very weak
    auto *b2 = new Territory(5, "Benev2", c2, 7,  p_benevolent);  // medium
    auto *b3 = new Territory(6, "Benev3", c2, 12, p_benevolent);  // strong
    
    //add adjencency for benevolent demo
    b1->addAdjacentTerritory(b2);
    b2->addAdjacentTerritory(b1);
    b2->addAdjacentTerritory(b3);
    b3->addAdjacentTerritory(b2);


    // Assign strategies
    p_human->setStrategy(new HumanPlayerStrategy());
    p_aggressive->setStrategy(new AggressivePlayerStrategy());
    // p_neutral->setStrategy(new NeutralPlayerStrategy());
    p_benevolent->setStrategy(new BenevolentPlayerStrategy());



    // Assign reinforcements
    p_human->addReinforcements(5);
    p_aggressive->addReinforcements(5);
    // p_neutral->addReinforcements(5);
    p_benevolent->addReinforcements(8);



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


    //assign territories to Benevolent:
    p_benevolent->addTerritory(b1);
    p_benevolent->addTerritory(b2);
    p_benevolent->addTerritory(b3);



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

    // =========================
    // (D) Benevolent Player (Automatic)
    // =========================
    std::cout << "\n--- Issuing orders for: " << p_benevolent->getName()
              << " (" << p_benevolent->getStrategy()->getType() << ") ---\n";
    std::cout << "(This should be automatic, reinforcing weakest territories and never attacking.)\n";
    p_benevolent->issueOrder(gameDeck);

    delete p_human;
    delete p_aggressive;
    // delete p_neutral;
    delete p_benevolent;


    // delete the territories we created
    delete t1;
    delete t2;
    delete t3;
    delete b1;
    delete b2;
    delete b3;

    delete c1;
    delete c2;

    // delete the deck
    delete gameDeck;

    std::cout << "Cleanup complete. Exiting." << std::endl;
}

int main()
{
    testPlayerStrategies();

    return 0;
}