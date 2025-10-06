/**
 * PlayerDriver.cpp
 * @author Gorden Quach 402643250
 * @date September 29th, 2025
 */

#include "Player.h"
#include "../Map/Map.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"
#include "../Cards/Cards.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

void testPlayers()
{
  cout << "Testing Player: " << endl;

  // creating armies:
  int *a1 = new int(10);
  int *a2 = new int(20);
  int *a3 = new int(30);

  // creating new map
  auto *c1 = new Continent("Cont1", 5);
  auto *t1 = new Territory(1, "tier1", c1, a1);
  auto *t2 = new Territory(2, "tier2", c1, a2);
  auto *t3 = new Territory(3, "tier3", c1, a3);

  // adding adjacency
  t1->addAdjacentTerritory(t2);
  t2->addAdjacentTerritory(t3);
  t3->addAdjacentTerritory(t1);

  // create a player
  Player p("Bob");
  p.addTerritory(t1);
  p.addTerritory(t2);
  p.addTerritory(t3);

  // Create Hands
  auto *h1 = new Hand();
  auto *h2 = new Hand();

  p.setHand(h1);

  // give orders list
  auto *ol = new OrdersList();
  p.setOrdersList(ol);

  // show info
  cout << p << endl;

  // testing functions
  auto defend = p.toDefend();
  auto attack = p.toAttack();

  // get territory name needed to defend
  cout << "to defend: ";
  for (auto *tt : defend)
  {
    cout << tt->getName() << " ";
  }
  cout << endl;

  // get territory name needed to attack
  cout << "to defend: ";
  for (auto *tt : attack)
  {
    cout << tt->getName() << " ";
  }
  cout << endl;

  // test issueOrder()

  p.issueOrder();
  cout << *p.getOrdersList() << endl;

  // copy constructor test

  Player q = p;
  cout << "copy Player q: " << q << endl;

  // setting hand for Player q.
  q.setHand(h2);

  // test copy assign
  Player r("Paul");
  r = q;
  cout << "copy assigned Player r: " << r << endl;

  // Memory cleanup:
  delete t1;
  t1 = nullptr;
  delete t2;
  t2 = nullptr;
  delete t3;
  t3 = nullptr;
  delete c1;
  c1 = nullptr;

  delete a1;
  delete a2;
  delete a3;
  a1 = nullptr;
  a2 = nullptr;
  a3 = nullptr;

  cout << "=== End of Driver. ===" << endl;
}

// int main() {
//   testPlayers();
// }