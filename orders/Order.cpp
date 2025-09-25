// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include "Order.h"
#include "../Map/Map.h"

using namespace std;

// Overloading the stream insertion operator
std::ostream &operator<<(ostream &os, const Order &order)
{
  order.print(os);
  return os;
}

// ======================== ORDER ======================== //

Order::Order(int i)
    : id{new int(i)}
{
}

Order::~Order()
{
  delete id;
  id = nullptr;
}

void Order::print(std::ostream &os) const
{
  os << "id: " << *id; // TODO: add player
}

int Order::getId()
{
  return *id;
}

bool Order::validate()
{
  cout << "Validating generic order... ID = " << *id << endl;
  return 1;
}

int Order::execute()
{
  cout << "Executing generic order... ID = " << *id << endl;
  return 0;
}

// ======================== DEPLOY ======================== //

Deploy::Deploy(int i, int n, Territory *t)
    : Order{i},
      numTroops{new int(n)},
      targetTerritory{t}
{
}

Deploy::~Deploy()
{
  delete numTroops;
  delete targetTerritory;
  numTroops = nullptr;
  targetTerritory = nullptr;
}

void Deploy::print(std::ostream &os) const
{
  Order::print(os);
  os << " | numTroops: " << *numTroops << " | targetTerritory: " << targetTerritory->getName();
}

bool Deploy::validate()
{
  // CHECKS:
  // numTroops <= # troops in reserve
  // Player controls targetTerritory
  return 1;
}

int Deploy::execute()
{
  // Add numTroops to targetTerritory
  return 0;
}

// ======================== ADVANCE ======================== //

Advance::Advance(int i, int n, Territory *s, Territory *t)
    : Order{i},
      numTroops{new int(n)},
      sourceTerritory{s},
      targetTerritory{t}
{
}

Advance::~Advance()
{
  delete numTroops;
  delete sourceTerritory;
  delete targetTerritory;
  numTroops = nullptr;
  sourceTerritory = nullptr;
  targetTerritory = nullptr;
}

bool Advance::validate()
{
  // CHECKS:
  // numtroops <= # troops in sourceTerritory
  // sourceTerritory and targetTerritory are adjacent
  // Player must control sourceTerritory
  // Controllers of sourceTerritory and targetTerritory must not be in Negotiation state
  return 1;
}

int Advance::execute()
{
  // Move troops from sourceTerritory to targetTerritory
  // Run combat calculations if needed
  return 0;
}

// ======================== BOMB ======================== //

Bomb::Bomb(int i, Territory *t)
    : Order{i},
      targetTerritory{t}
{
}

Bomb::~Bomb()
{
  delete targetTerritory;
  targetTerritory = nullptr;
}

bool Bomb::validate()
{
  // CHECKS:
  // Player must have a Bomb card in hand
  // Controller of targetTerritory and the current player must not be in Negotiation state
  return 1;
}

int Bomb::execute()
{
  // Halve the number of troops in targetTerritory
  return 0;
}

// ======================== BLOCKADE ======================== //

Blockade::Blockade(int i, Territory *t)
    : Order{i},
      targetTerritory{t}
{
}

Blockade::~Blockade()
{
  delete targetTerritory;
  targetTerritory = nullptr;
}

bool Blockade::validate()
{
  // CHECKS:
  // Player must have a Blockade card in hand
  return 1;
}

int Blockade::execute()
{
  // Triple the number of troops in targetTerritory
  // Set targetTerritory status to neutral
  return 0;
}

// ======================== AIRLIFT ======================== //

Airlift::Airlift(int i, int n, Territory *s, Territory *t)
    : Order{i},
      numTroops{new int(n)},
      sourceTerritory{s},
      targetTerritory{t}
{
}

Airlift::~Airlift()
{
  delete numTroops;
  delete sourceTerritory;
  delete targetTerritory;
  numTroops = nullptr;
  sourceTerritory = nullptr;
  targetTerritory = nullptr;
}

bool Airlift::validate()
{
  // CHECKS:
  // Player must have an Airlift card in hand
  // numTroops <= # troops in sourceTerritory
  // Player must control sourceTerritory
  // Controllers of sourceTerritory and targetTerritory must not be in Negotiation state
  return 1;
}

int Airlift::execute()
{
  // Move numTroops from sourceTerritory to targetTerritory
  // Run combat calculations if needed
  return 0;
}

// ======================== NEGOTIATE ======================== //

Negotiate::Negotiate(int i, string p)
    : Order{i},
      targetPlayer{&p} // TODO: change to player
{
}

Negotiate::~Negotiate()
{
  delete targetPlayer;
  targetPlayer = nullptr;
}

bool Negotiate::validate()
{
  // CHECKS:
  // Player must have a Negotiate card in hand
  return 1;
}

int Negotiate::execute()
{
  // Impose Negotiation state between the current player and targetPlayer for one turn
  return 0;
}