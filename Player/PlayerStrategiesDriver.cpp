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
    Player *p_human = new Player("Elian");
    Player *p_aggressive = new Player("Gabe");
    Player *p_benevolent = new Player("Gorden");
    Player *p_neutral = new Player("Paul");
    Player *p_cheater = new Player("Matthew");

    std::vector<Player*> players = {p_human, p_neutral, p_aggressive, p_benevolent, p_cheater};

    // creating new map
    auto *c1 = new Continent("Cont1", 5);
    auto *t1 = new Territory(1, "tier1", c1, 10, p_human);
    auto *t2 = new Territory(2, "tier2", c1, 20, p_human);
    auto *t3 = new Territory(3, "tier3", c1, 30, p_human);

    auto *c2 = new Continent("Cont2", 5);
    auto *b1 = new Territory(4, "Benev1", c2, 2,  p_benevolent);  // very weak
    auto *b2 = new Territory(5, "Benev2", c2, 7,  p_benevolent);  // medium
    auto *b3 = new Territory(6, "Benev3", c2, 12, p_benevolent);  // strong
    
    auto *c3 = new Continent("Cont3", 5);
    auto *d1 = new Territory(1, "aggr1", c3, 10, p_aggressive);
    auto *d2 = new Territory(2, "aggr2", c3, 20, p_aggressive);
    auto *d3 = new Territory(3, "aggr3", c3, 12, p_aggressive);

    auto *d4 = new Territory(3, "neutral1", c3, 15, p_neutral);
    auto *d5 = new Territory(3, "neutral2", c3, 33, p_neutral);
    auto *d6 = new Territory(3, "neutral3", c3, 25, p_neutral);

    auto *d7 = new Territory(3, "cheater1", c3, 7, p_cheater);
    auto *d8 = new Territory(3, "cheater2", c3, 8, p_cheater);
    auto *d9 = new Territory(3, "cheater3", c3, 9, p_cheater);

    // Assign to players
    p_human->addTerritory(t1);
    p_human->addTerritory(t2);
    p_human->addTerritory(t3);

    p_benevolent->addTerritory(b1);
    p_benevolent->addTerritory(b2);
    p_benevolent->addTerritory(b3);

    p_aggressive->addTerritory(d1);
    p_aggressive->addTerritory(d2);
    p_aggressive->addTerritory(d3);

    p_neutral->addTerritory(d4);
    p_neutral->addTerritory(d5);
    p_neutral->addTerritory(d6);

    p_cheater->addTerritory(d7);
    p_cheater->addTerritory(d8);
    p_cheater->addTerritory(d9);

    // Map adjacencies
    t1->addAdjacentTerritory(t2); t2->addAdjacentTerritory(t1);
    t2->addAdjacentTerritory(t3); t3->addAdjacentTerritory(t2);
    t1->addAdjacentTerritory(t3); t3->addAdjacentTerritory(t1);

    b1->addAdjacentTerritory(b2); b2->addAdjacentTerritory(b1);
    b2->addAdjacentTerritory(b3); b3->addAdjacentTerritory(b2);
    b1->addAdjacentTerritory(b3); b3->addAdjacentTerritory(b1);

    std::vector<Territory*> continent3 = {d1, d2, d3, d4, d5, d6, d7, d8, d9};

    for (size_t i = 0; i < continent3.size(); ++i) {
        for (size_t j = i + 1; j < continent3.size(); ++j) {
            continent3[i]->addAdjacentTerritory(continent3[j]);
            continent3[j]->addAdjacentTerritory(continent3[i]);
        }
    }

    t1->addAdjacentTerritory(d4); d4->addAdjacentTerritory(t1);
    t2->addAdjacentTerritory(d5); d5->addAdjacentTerritory(t2);
    t3->addAdjacentTerritory(d6); d6->addAdjacentTerritory(t3);

    t3->addAdjacentTerritory(d1); d1->addAdjacentTerritory(t3);
    t2->addAdjacentTerritory(d2); d2->addAdjacentTerritory(t2);
    b1->addAdjacentTerritory(d9); d9->addAdjacentTerritory(b1);
    b3->addAdjacentTerritory(d7); d7->addAdjacentTerritory(b3);

    // Assign strategies
    p_human->setStrategy(new HumanPlayerStrategy());
    p_aggressive->setStrategy(new AggressivePlayerStrategy());
    p_neutral->setStrategy(new NeutralPlayerStrategy());
    p_benevolent->setStrategy(new BenevolentPlayerStrategy());
    p_cheater->setStrategy(new CheaterPlayerStrategy());

    // Assign reinforcements
    p_human->addReinforcements(5);
    p_aggressive->addReinforcements(5);
    p_cheater->addReinforcements(5);
    p_neutral->addReinforcements(5);
    p_benevolent->addReinforcements(5);


    for (int i = 0; i < 2; i++) {
        std::cout << "\n*****************************"<< std::endl;
        std::cout << "      ROUND " << (i + 1) << " - ISSUE ORDERS" << std::endl;
        std::cout << "*******************************"<< std::endl;

        // (D) Cheater Player (Automatic)
        std::cout << "\n--- Issuing orders for: " << p_cheater->getName()
                << " (" << p_cheater->getStrategy()->getType() << ") ---" << std::endl;
        std::cout << "(This should be automatic, simply getting territories)" << std::endl;
        p_cheater->issueOrder(gameDeck);

        // (A) Human Player (Interactive)
        std::cout << "\n--- Issuing orders for: " << p_human->getName()
                << " (" << p_human->getStrategy()->getType() << ") ---" << std::endl;
        p_human->issueOrder(gameDeck);
        std::cout << "\nHuman player has finished issuing orders." << std::endl;

        // (B) Neutral Player (Automatic)
        std::cout << "\n--- Issuing orders for: " << p_neutral->getName()
                << " (" << p_neutral->getStrategy()->getType() << ") ---" << std::endl;
        std::cout << "(This should be automatic and do nothing...)" << std::endl;
        p_neutral->issueOrder(gameDeck);

        // (C) Aggressive Player (Automatic)
        std::cout << "\n--- Issuing orders for: " << p_aggressive->getName()
                << " (" << p_aggressive->getStrategy()->getType() << ") ---" << std::endl;
        std::cout << "(This should be automatic, focusing on attacking...)" << std::endl;
        p_aggressive->issueOrder(gameDeck);

        // (D) Benevolent Player (Automatic)
        std::cout << "\n--- Issuing orders for: " << p_benevolent->getName()
                << " (" << p_benevolent->getStrategy()->getType() << ") ---\n";
        std::cout << "(This should be automatic, reinforcing weakest territories and never attacking.)\n";
        p_benevolent->issueOrder(gameDeck);

        // same as execute phase in GameEngine
        std::cout << "\n*****************************"<< std::endl;
        std::cout << "      ROUND " << (i + 1) << " - EXECUTE ORDERS" << std::endl;
        std::cout << "*******************************"<< std::endl;
        bool ordersRemaining = true;
        while (ordersRemaining) {
            ordersRemaining = false;

            for (auto& player : players) {
                OrdersList* list = player->getOrdersList();
                auto* orders = list->getOrders();

                if (!orders->empty()) {
                    ordersRemaining = true;

                    // Get the order to execute
                    Order* order = orders->front();
                    int orderId = order->getId();

                    // EXECUTE ORDER
                    int result = order->execute();

                    if (result == 0) {
                        std::cout << "\t" << player->getName() << " - Execute: " << *order << "\n";
                    }
                    else {
                        std::cerr << "\t" << player->getName() << " - Failed to execute: " << *order << "\n";
                    }

                    // Remove from player's orders list
                    int removeResult = list->remove(orderId);
                    if (removeResult != 0) {
                        std::cerr << "Failed to remove order ID: " << orderId;
                    }
                }
            }
        }

    std::cout << "\tALL ORDERS EXECUTED\n\n";
    }

    // delete the territories we created
    delete t1;
    delete t2;
    delete t3;
    delete b1;
    delete b2;
    delete b3;
    delete d1;
    delete d2;
    delete d3;
    delete d4;
    delete d5;
    delete d6;
    delete d7;
    delete d8;
    delete d9;

    delete c1;
    delete c2;
    delete c3;

    delete p_human;
    delete p_aggressive;
    delete p_neutral;
    delete p_benevolent;
    delete p_cheater;
    
    // delete the deck
    delete gameDeck;

    std::cout << "Cleanup complete. Exiting." << std::endl;
}

int main()
{
    testPlayerStrategies();

    return 0;
}