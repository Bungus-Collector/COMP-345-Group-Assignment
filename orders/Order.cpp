// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include "Order.h"
#include "OrdersErrorCodes.h"
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

Order::Order(const Order &other)
{
  this->id = new int(*other.id);
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

Order &Order::operator=(const Order &other)
{
  if (this == &other)
  {
    return *this;
  }

  delete id;

  this->id = new int(*other.id);
  return *this;
}

int Order::getId()
{
  return *id;
}

bool Order::validate()
{
  cout << "Validating generic order... ID = " << *id << endl;
  return true;
}

int Order::execute()
{
  cout << "Executing generic order... ID = " << *id << endl;
  return SUCCESS;
}

// ======================== DEPLOY ======================== //

Deploy::Deploy(int i, int n, Territory *t)
    : Order{i},
      numTroops{new int(n)},
      targetTerritory{t}
{
}

Deploy::Deploy(const Deploy &other) : Order(other)
{
  this->numTroops = new int(*other.numTroops);
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Deploy::~Deploy()
{
  delete numTroops;
  numTroops = nullptr;
  targetTerritory = nullptr;
}

void Deploy::print(std::ostream &os) const
{
  Order::print(os);
  os << " | numTroops: " << *numTroops << " | targetTerritory: " << targetTerritory->getName();
}

Deploy &Deploy::operator=(const Deploy &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);
  delete numTroops;
  delete targetTerritory;

  this->numTroops = new int(*other.numTroops);
  this->targetTerritory = new Territory(*other.targetTerritory);
  return *this;
}

bool Deploy::validate()
{
  // CHECKS:
  // numTroops <= # troops in reserve
  // Player controls targetTerritory
  return true;
}

int Deploy::execute()
{
  // Add numTroops to targetTerritory
  return SUCCESS;
}

// ======================== ADVANCE ======================== //

Advance::Advance(int i, int n, Territory *s, Territory *t)
    : Order{i},
      numTroops{new int(n)},
      sourceTerritory{s},
      targetTerritory{t}
{
}

Advance::Advance(const Advance &other) : Order(other)
{
  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = new Territory(*other.sourceTerritory);
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Advance::~Advance()
{
  delete numTroops;
  numTroops = nullptr;
  sourceTerritory = nullptr;
  targetTerritory = nullptr;
}

void Advance::print(std::ostream &os) const
{
  Order::print(os);
  os << " | numTroops: " << *numTroops
     << " | sourceTerritory: " << sourceTerritory->getName()
     << " | targetTerritory: " << targetTerritory->getName();
}

Advance &Advance::operator=(const Advance &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);
  delete numTroops;
  delete sourceTerritory;
  delete targetTerritory;

  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = new Territory(*other.sourceTerritory);
  this->targetTerritory = new Territory(*other.targetTerritory);
  return *this;
}

bool Advance::validate()
{
  // CHECKS:
  // numtroops <= # troops in sourceTerritory
  // sourceTerritory and targetTerritory are adjacent
  // Player must control sourceTerritory
  // Controllers of sourceTerritory and targetTerritory must not be in Negotiation state
  return true;
}

int Advance::execute()
{
  // Move troops from sourceTerritory to targetTerritory
  // Run combat calculations if needed
  return SUCCESS;
}

// ======================== BOMB ======================== //

Bomb::Bomb(int i, Territory *t)
    : Order{i},
      targetTerritory{t}
{
}

Bomb::Bomb(const Bomb &other) : Order(other)
{
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Bomb::~Bomb()
{
  targetTerritory = nullptr;
}

void Bomb::print(std::ostream &os) const
{
  Order::print(os);
  os << " | targetTerritory: " << targetTerritory->getName();
}

Bomb &Bomb::operator=(const Bomb &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);
  delete targetTerritory;

  this->targetTerritory = new Territory(*other.targetTerritory);
  return *this;
}

bool Bomb::validate()
{
  // CHECKS:
  // Player must have a Bomb card in hand
  // Controller of targetTerritory and the current player must not be in Negotiation state
  return true;
}

int Bomb::execute()
{
  // Halve the number of troops in targetTerritory
  return SUCCESS;
}

// ======================== BLOCKADE ======================== //

Blockade::Blockade(int i, Territory *t)
    : Order{i},
      targetTerritory{t}
{
}

Blockade::Blockade(const Blockade &other) : Order(other)
{
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Blockade::~Blockade()
{
  targetTerritory = nullptr;
}

void Blockade::print(std::ostream &os) const
{
  Order::print(os);
  os << " | targetTerritory: " << targetTerritory->getName();
}

Blockade &Blockade::operator=(const Blockade &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);
  delete targetTerritory;

  this->targetTerritory = new Territory(*other.targetTerritory);
  return *this;
}

bool Blockade::validate()
{
  // CHECKS:
  // Player must have a Blockade card in hand
  return true;
}

int Blockade::execute()
{
  // Triple the number of troops in targetTerritory
  // Set targetTerritory status to neutral
  return SUCCESS;
}

// ======================== AIRLIFT ======================== //

Airlift::Airlift(int i, int n, Territory *s, Territory *t)
    : Order{i},
      numTroops{new int(n)},
      sourceTerritory{s},
      targetTerritory{t}
{
}

Airlift::Airlift(const Airlift &other) : Order(other)
{
  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = new Territory(*other.sourceTerritory);
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Airlift::~Airlift()
{
  delete numTroops;
  numTroops = nullptr;
  sourceTerritory = nullptr;
  targetTerritory = nullptr;
}

void Airlift::print(std::ostream &os) const
{
  Order::print(os);
  os << " | numTroops: " << *numTroops
     << " | sourceTerritory: " << sourceTerritory->getName()
     << " | targetTerritory: " << targetTerritory->getName();
}

Airlift &Airlift::operator=(const Airlift &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);
  delete numTroops;
  delete sourceTerritory;
  delete targetTerritory;

  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = new Territory(*other.sourceTerritory);
  this->targetTerritory = new Territory(*other.targetTerritory);
  return *this;
}

bool Airlift::validate()
{
  // CHECKS:
  // Player must have an Airlift card in hand
  // numTroops <= # troops in sourceTerritory
  // Player must control sourceTerritory
  // Controllers of sourceTerritory and targetTerritory must not be in Negotiation state
  return true;
}

int Airlift::execute()
{
  // Move numTroops from sourceTerritory to targetTerritory
  // Run combat calculations if needed
  return SUCCESS;
}

// ======================== NEGOTIATE ======================== //

Negotiate::Negotiate(int i, string p)
    : Order{i},
      targetPlayer{&p} // TODO: change to player
{
}

Negotiate::Negotiate(const Negotiate &other) : Order(other)
{
  this->targetPlayer = new string(*other.targetPlayer);
}

Negotiate::~Negotiate()
{
  targetPlayer = nullptr;
}

void Negotiate::print(std::ostream &os) const
{
  Order::print(os);
  os << " | numTroops: " << *targetPlayer;
}

Negotiate &Negotiate::operator=(const Negotiate &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);
  delete targetPlayer;

  this->targetPlayer = new string(*other.targetPlayer);
  return *this;
}

bool Negotiate::validate()
{
  // CHECKS:
  // Player must have a Negotiate card in hand
  return true;
}

int Negotiate::execute()
{
  // Impose Negotiation state between the current player and targetPlayer for one turn
  return SUCCESS;
}