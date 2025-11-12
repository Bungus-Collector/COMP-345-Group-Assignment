// Author: Gabriel Lagacé

#include <iostream>
#include "Order.h"
#include "OrdersList.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

/**
 * Old test function for assignment 1
 * Outdated as of assignment 2
 *
 * Use testOrderExecution() function
 */
void testOrdersList()
{
    // cout << "===================================" << endl;
    // cout << "            Orders Test            " << endl;
    // cout << "===================================" << endl;

    // // Constructors
    // OrdersList *testList1 = new OrdersList();

    // Order *order1 = new Order(1);
    // Order *order2 = new Order(2);
    // Deploy *order3 = new Deploy(3, 10, new Territory(1, "Territory A", new Continent("Gabeland", 10), new int(10)));

    // Order *order4 = new Order(*order1);
    // Deploy *order5 = new Deploy(*order3);

    // // Add function
    // testList1->add(order1);
    // testList1->add(order2);
    // testList1->add(order3);
    // testList1->add(order4);
    // testList1->add(order5);

    // // Move function
    // testList1->move(order1, 2);

    // // Execute deploy order
    // cout << "Deploy order test execution: Before execution" << endl;
    // cout << *order5->getTargetTerritory()->getArmies() << endl;
    // order5->execute();
    // cout << "Deploy order test execution: After execution" << endl;
    // cout << *order5->getTargetTerritory()->getArmies() << endl;

    // // Before removal
    // cout << "Remove order from list: Before execution" << endl;
    // cout << *testList1 << endl;
    // cout << "\n"
    //      << *order1 << endl;

    // testList1->remove(order1);

    // // After removal
    // cout << "Remove order from list: After execution" << endl;
    // cout << *testList1 << endl;
    // cout << "\n"
    //      << *order1 << endl;

    // // Assignment operator
    // OrdersList testList2 = *testList1;

    // list<Order *> *list1 = testList1->getOrders();
    // list<Order *> *list2 = testList2.getOrders();

    // cout << "Creating deep copy with assignment operator" << endl;
    // cout << "list1:" << endl;
    // for (Order *o : *list1)
    // {
    //     cout << o << " ";
    // }
    // cout << endl;

    // cout << "list2:" << endl;
    // for (Order *o : *list2)
    // {
    //     cout << o << " ";
    // }
    // cout << endl;

    // Order order6 = *order1;

    // cout << order1 << " " << &order6 << endl;
}

void testOrderExecution()
{
    cout << "===================================" << endl;
    cout << "      Orders Execution Test        " << endl;
    cout << "===================================" << endl;

    cout << "Preparing territories and players..." << endl;
    Continent *c1 = new Continent("Continent 1", 10);
    Player *p1 = new Player("Jensen");
    Player *p2 = new Player("Pritchard");
    Territory *t1 = new Territory(1, "Territory 1", c1, 0, p1);
    Territory *t2 = new Territory(2, "Territory 2", c1, 0, p2);
    Territory *t3 = new Territory(3, "Territory 3", c1, 10, p1);
    Territory *t4 = new Territory(2, "Territory 4", c1, 10, p2);
    t1->addAdjacentTerritory(t2);
    t2->addAdjacentTerritory(t1);
    t3->addAdjacentTerritory(t4);
    t4->addAdjacentTerritory(t3);

    cout << "Populating territories..." << endl;
    int t1Armies, t2Armies;
    cout << "Enter number of armies in player 1 territory (attacker): " << endl;
    cin >> t1Armies;
    cout << "Enter number of armies in player 2 territory (defender): " << endl;
    cin >> t2Armies;
    t1->setArmies(t1Armies);
    t2->setArmies(t2Armies);

    cout << "======================== Deploy Order ========================" << endl;
    int reinforcements;
    cout << "Enter the number of troops to deploy: " << endl;
    cin >> reinforcements;

    Deploy *deployOrder_valid = new Deploy(0, p1, reinforcements, t1);
    Deploy *deployOrder_invalid = new Deploy(0, p1, reinforcements, t2);

    cout << "Executing INVALID deploy order (issuer does not control target territory)..." << endl;
    cout << "Troops in t2 (before): " << t2->getArmies() << endl;
    deployOrder_invalid->execute();
    cout << "Troops in t2 (after): " << t2->getArmies() << endl;
    cout << endl;

    cout << "Executing VALID deploy order..." << endl;
    cout << "Troops in t1 (before): " << t1->getArmies() << endl;
    deployOrder_valid->execute();
    cout << "Troops in t1 (after): " << t1->getArmies() << endl;

    cout << "======================== Advance Order ========================" << endl;
    int attackers;
    cout << "Enter the number of troops to advance: " << endl;
    cin >> attackers;

    Advance *advanceOrder_valid = new Advance(0, p1, attackers, t1, t2);
    Advance *advanceOrder_invalid = new Advance(0, p1, attackers + t1->getArmies(), t1, t2);

    cout << "Executing INVALID advance order (advancing armies exceeds reserves in source territory)..." << endl;
    cout << "Troops in t1 (before): " << t1->getArmies() << " (" << *advanceOrder_invalid->getNumTroops() << " attacking)" << endl;
    cout << "Troops in t2 (before): " << t2->getArmies() << endl;
    cout << "t2 owner (before): " << t2->getOwner()->getName() << endl;
    advanceOrder_invalid->execute();
    cout << "Troops in t1 (after): " << t1->getArmies() << endl;
    cout << "Troops in t2 (after): " << t2->getArmies() << endl;
    cout << "t2 owner (after): " << t2->getOwner()->getName() << endl;
    cout << endl;

    cout << "Executing VALID advance order..." << endl;
    cout << "Troops in t1 (before): " << t1->getArmies() << " (" << *advanceOrder_valid->getNumTroops() << " attacking)" << endl;
    cout << "Troops in t2 (before): " << t2->getArmies() << endl;
    cout << "t2 owner (before): " << t2->getOwner()->getName() << endl;
    advanceOrder_valid->execute();
    cout << "Troops in t1 (after): " << t1->getArmies() << endl;
    cout << "Troops in t2 (after): " << t2->getArmies() << endl;
    cout << "t2 owner (after): " << t2->getOwner()->getName() << endl;

    cout << "======================== Bomb Order ========================" << endl;
    Bomb *bombOrder_valid = new Bomb(0, p1, t4);
    Bomb *bombOrder_invalid = new Bomb(0, p1, t1);

    cout << "Executing INVALID bomb order (player 1 attempting to bomb one of his own territories)..." << endl;
    cout << "Troops in t1 (before): " << t1->getArmies() << endl;
    bombOrder_invalid->execute();
    cout << "Troops in t1 (after): " << t1->getArmies() << endl;
    cout << endl;

    cout << "Executing VALID bomb order..." << endl;
    cout << "Troops in t4 (before): " << t4->getArmies() << endl;
    bombOrder_valid->execute();
    cout << "Troops in t4 (after): " << t4->getArmies() << endl;

    cout << "======================== Airlift Order ========================" << endl;
    int airdrop;
    cout << "Enter the number of troops to airlift: " << endl;
    cin >> airdrop;

    Airlift *airliftOrder_valid = new Airlift(0, p1, airdrop, t1, t3); // Note that t3 is not adjacent to t1
    Airlift *airliftOrder_invalid = new Airlift(0, p1, airdrop, t1, t4);

    cout << "Executing INVALID airlift order (player 1 attempting to airlift into player 2 territory)..." << endl;
    cout << "Troops in t4 (before): " << t4->getArmies() << endl;
    airliftOrder_invalid->execute();
    cout << "Troops in t4 (after): " << t4->getArmies() << endl;
    cout << endl;

    cout << "Executing valid airlift order..." << endl;
    cout << "Troops in t3 (before): " << t3->getArmies() << endl;
    airliftOrder_valid->execute();
    cout << "Troops in t3 (after): " << t3->getArmies() << endl;

    cout << "======================== Blockade Order ========================" << endl;
    Blockade *blockadeOrder_valid = new Blockade(0, p1, t3);
    Blockade *blockadeOrder_invalid = new Blockade(0, p1, t4);

    cout << "Executing INVALID blockade order (player 1 attempting to blockade player 2 territory)..." << endl;
    cout << "Troops in t4 (before): " << t4->getArmies() << endl;
    cout << "t4 owner (before): " << t4->getOwner()->getName() << endl;
    blockadeOrder_invalid->execute();
    cout << "Troops in t4 (after): " << t4->getArmies() << endl;
    cout << "t4 owner (after): " << t4->getOwner()->getName() << endl;
    cout << endl;

    cout << "Executing VALID blockade order..." << endl;
    cout << "Troops in t3 (before): " << t3->getArmies() << endl;
    cout << "t3 owner (before): " << t3->getOwner()->getName() << endl;
    blockadeOrder_valid->execute();
    cout << "Troops in t3 (after): " << t3->getArmies() << endl;
    cout << "t3 owner (after): " << t3->getOwner()->getName() << endl;

    cout << "======================== Negotiate Order ========================" << endl;
    Negotiate *negotiateOrder_valid = new Negotiate(0, p1, p2);
    Negotiate *negotiateOrder_invalid = new Negotiate(0, p1, p1);

    cout << "Executing INVALID negotiate order (player 1 attempting to negotiate with themselves)..." << endl;
    negotiateOrder_invalid->execute();
    cout << endl;

    cout << "Executing VALID negotiate order" << endl;
    negotiateOrder_valid->execute();
    cout << "Player 1 attempting to attack player 2 during negotiations:" << endl;
    t1->changeNumArmies(attackers);
    Advance *advanceOrder_negotiating = new Advance(0, p1, t3->getArmies(), t3, t4);
    advanceOrder_negotiating->execute();

    cout << "Cleaning up..." << endl;
    delete deployOrder_valid;
    delete deployOrder_invalid;
    delete advanceOrder_valid;
    delete advanceOrder_invalid;
    delete bombOrder_valid;
    delete bombOrder_invalid;
    delete airliftOrder_valid;
    delete airliftOrder_invalid;
    delete blockadeOrder_valid;
    delete blockadeOrder_invalid;
    delete negotiateOrder_valid;
    delete negotiateOrder_invalid;
    delete advanceOrder_negotiating;
    delete p1;
    delete p2;
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete c1;
}

// int main()
// {
//     testOrderExecution();

//     return 0;
// }