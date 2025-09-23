// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include "Order.h"

using namespace std;

// ORDER

Order::Order(int i)
    : id{&i}
{
}

Order::~Order()
{
  delete id;
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

// DEPLOY

Deploy::Deploy(int i, int n, string t)
    : Order{i},
      numTroops{&n},
      targetTerritory{&t}
{
}

Deploy::~Deploy()
{
  delete numTroops;
  delete targetTerritory;
}

bool Deploy::validate()
{
  // CHECKS:
  // numTroops <= # troops in reserve
  return 1;
}

int Deploy::execute()
{
  // Add numTroops to targetTerritory
  return 0;
}

// ADVANCE

Advance::Advance(int i, int n, string s, string t)
    : Order{i},
      numTroops{&n},
      sourceTerritory{&s},
      targetTerritory{&t}
{
}

Advance::~Advance()
{
  delete numTroops;
  delete sourceTerritory;
  delete targetTerritory;
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

// BOMB

Bomb::Bomb(int i, string t)
    : Order{i},
      targetTerritory{&t}
{
}

Bomb::~Bomb()
{
  delete targetTerritory;
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

// BLOCKADE

Blockade::Blockade(int i, string t)
    : Order{i},
      targetTerritory{&t}
{
}

Blockade::~Blockade()
{
  delete targetTerritory;
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

// AIRLIFT

Airlift::Airlift(int i, int n, string s, string t)
    : Order{i},
      numTroops{&n},
      sourceTerritory{&s},
      targetTerritory{&t}
{
}

Airlift::~Airlift()
{
  delete numTroops;
  delete sourceTerritory;
  delete targetTerritory;
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

// NEGOTIATE

Negotiate::Negotiate(int i, string p)
    : Order{i},
      targetPlayer{&p}
{
}

Negotiate::~Negotiate()
{
  delete targetPlayer;
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