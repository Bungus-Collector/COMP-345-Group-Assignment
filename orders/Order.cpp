// Author: Gabriel Lagacé

/**
 * Design note:
 * Deep copies of Player and Territory pointers are not made
 * in constructors or other assignments. This is because the
 * order needs to directly modify the values of that
 * specific Player/Territory. This is also why Players and
 * Territory members aren't deleted in destructors because
 * that would delete the object for the entire game.
 */

#include <iostream>
#include <string>
#include "Order.h"
#include "OrdersErrorCodes.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

using namespace std;

// Overloading the stream insertion operator
std::ostream &operator<<(ostream &os, const Order &order)
{
  order.print(os);
  return os;
}

// ======================== ORDER ======================== //

Order::Order()
    : id{new int()},
      issuer{new Player()}
{
}

Order::Order(int i, Player *p)
    : id{new int(i)},
      issuer{p}
{
}

Order::Order(const Order &other)
{
  this->id = new int(*other.id);
  this->issuer = new Player(*other.issuer);
}

Order::~Order()
{
  delete id;
  id = nullptr;
  issuer = nullptr;
}

void Order::print(std::ostream &os) const
{
  os << "id: " << *id << " | issuer: " << issuer->getName();
}

Order &Order::operator=(const Order &other)
{
  if (this == &other)
  {
    return *this;
  }

  delete id;

  this->id = new int(*other.id);
  this->issuer = other.issuer;
  return *this;
}

int Order::getId()
{
  return *id;
}

Player *Order::getIssuer()
{
  return issuer;
}

void Order::setId(int i)
{
  delete id;
  id = new int(i);
}

void Order::setIssuer(Player *p)
{
  issuer = p;
}

// ======================== DEPLOY ======================== //

Deploy::Deploy()
    : Order{},
      numTroops{new int()},
      targetTerritory{new Territory()}
{
}

Deploy::Deploy(int i, Player *p, int n, Territory *t)
    : Order{i, p},
      numTroops{new int(n)},
      targetTerritory{t}
{
}

Deploy::Deploy(const Deploy &other) : Order(other)
{
  this->numTroops = new int(*other.numTroops);
  this->targetTerritory = other.targetTerritory;
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

  this->numTroops = new int(*other.numTroops);
  this->targetTerritory = other.targetTerritory;
  return *this;
}

int *Deploy::getNumTroops() { return numTroops; }

Territory *Deploy::getTargetTerritory() { return targetTerritory; }

void Deploy::setNumTroops(int *n)
{
  delete numTroops;
  numTroops = n;
}

void Deploy::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Deploy::validate()
{
  // CHECKS:
  // Player controls targetTerritory

  return true;
}

int Deploy::execute()
{
  // Add numTroops to targetTerritory
  targetTerritory->setArmies(new int(*targetTerritory->getArmies() + *numTroops));
  return SUCCESS;
}

// ======================== ADVANCE ======================== //

Advance::Advance()
    : Order{},
      numTroops{new int()},
      sourceTerritory{new Territory()},
      targetTerritory{new Territory()}
{
}

Advance::Advance(int i, Player *p, int n, Territory *s, Territory *t)
    : Order{i, p},
      numTroops{new int(n)},
      sourceTerritory{s},
      targetTerritory{t}
{
}

Advance::Advance(const Advance &other) : Order(other)
{
  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = other.sourceTerritory;
  this->targetTerritory = other.targetTerritory;
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

int *Advance::getNumTroops() { return numTroops; }

Territory *Advance::getSourceTerritory() { return sourceTerritory; }

Territory *Advance::getTargetTerritory() { return targetTerritory; }

void Advance::setNumTroops(int *n)
{
  delete numTroops;
  numTroops = n;
}

void Advance::setSourceTerritory(Territory *t)
{
  delete sourceTerritory;
  sourceTerritory = t;
}

void Advance::setTargetTerritory(Territory *t)
{
  delete targetTerritory;
  targetTerritory = t;
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
  sourceTerritory->changeNumArmies(-(*numTroops));
  targetTerritory->changeNumArmies(*numTroops);
  return SUCCESS;
}

// ======================== BOMB ======================== //

Bomb::Bomb()
    : Order{},
      targetTerritory{new Territory()}
{
}

Bomb::Bomb(int i, Player *p, Territory *t)
    : Order{i, p},
      targetTerritory{t}
{
}

Bomb::Bomb(const Bomb &other) : Order(other)
{
  this->targetTerritory = other.targetTerritory;
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

  this->targetTerritory = other.targetTerritory;
  return *this;
}

Territory *Bomb::getTargetTerritory() { return targetTerritory; }

void Bomb::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Bomb::validate()
{
  // CHECKS:
  // Controller of targetTerritory and the current player must not be in Negotiation state
  return true;
}

int Bomb::execute()
{
  // Halve the number of troops in targetTerritory
  targetTerritory->changeNumArmies(-(*targetTerritory->getArmies() / 2));
  return SUCCESS;
}

// ======================== BLOCKADE ======================== //

Blockade::Blockade()
    : Order{},
      targetTerritory{new Territory()}
{
}

Blockade::Blockade(int i, Player *p, Territory *t)
    : Order{i, p},
      targetTerritory{t}
{
}

Blockade::Blockade(const Blockade &other) : Order(other)
{
  this->targetTerritory = other.targetTerritory;
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

  this->targetTerritory = other.targetTerritory;
  return *this;
}

Territory *Blockade::getTargetTerritory() { return targetTerritory; }

void Blockade::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Blockade::validate()
{
  // CHECKS:
  // targetTerritory must not be controlled by an enemy player
  return true;
}

int Blockade::execute()
{
  // Triple the number of troops in targetTerritory
  // TODO: Set targetTerritory status to neutral
  targetTerritory->changeNumArmies(*(targetTerritory->getArmies()) * 3);
  return SUCCESS;
}

// ======================== AIRLIFT ======================== //

Airlift::Airlift()
    : Order{},
      numTroops{new int()},
      sourceTerritory{new Territory()},
      targetTerritory{new Territory()}
{
}

Airlift::Airlift(int i, Player *p, int n, Territory *s, Territory *t)
    : Order{i, p},
      numTroops{new int(n)},
      sourceTerritory{s},
      targetTerritory{t}
{
}

Airlift::Airlift(const Airlift &other) : Order(other)
{
  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = other.sourceTerritory;
  this->targetTerritory = other.targetTerritory;
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

  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = other.sourceTerritory;
  this->targetTerritory = other.targetTerritory;
  return *this;
}

int *Airlift::getNumTroops() { return numTroops; }

Territory *Airlift::getSourceTerritory() { return sourceTerritory; }

Territory *Airlift::getTargetTerritory() { return targetTerritory; }

void Airlift::setNumTroops(int *n)
{
  delete numTroops;
  numTroops = n;
}

void Airlift::setSourceTerritory(Territory *t)
{
  sourceTerritory;
}

void Airlift::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Airlift::validate()
{
  // CHECKS:
  // numTroops <= # troops in sourceTerritory
  // Player must control sourceTerritory
  // Controllers of sourceTerritory and targetTerritory must not be in Negotiation state
  return true;
}

int Airlift::execute()
{
  // Move numTroops from sourceTerritory to targetTerritory
  // TODO: Run combat calculations if needed
  sourceTerritory->changeNumArmies(-(*numTroops));
  targetTerritory->changeNumArmies(*numTroops);
  return SUCCESS;
}

// ======================== NEGOTIATE ======================== //

Negotiate::Negotiate()
    : Order{},
      targetPlayer{new Player()}
{
}

Negotiate::Negotiate(int i, Player *p, Player *t)
    : Order{i, p},
      targetPlayer{t} // TODO: change to player
{
}

Negotiate::Negotiate(const Negotiate &other) : Order(other)
{
  this->targetPlayer = other.targetPlayer;
}

Negotiate::~Negotiate()
{
  targetPlayer = nullptr;
}

void Negotiate::print(std::ostream &os) const
{
  Order::print(os);
  os << " | targetPlayer: " << targetPlayer->getName();
}

Negotiate &Negotiate::operator=(const Negotiate &other)
{
  if (this == &other)
  {
    return *this;
  }

  Order::operator=(other);

  this->targetPlayer = other.targetPlayer;
  return *this;
}

Player *Negotiate::getTargetPlayer() { return targetPlayer; }

void Negotiate::setTargetPlayer(Player *p)
{
  targetPlayer = p;
}

bool Negotiate::validate()
{
  // CHECKS:
  // If source player and target player are the same then order is invalid
  return true;
}

int Negotiate::execute()
{
  // Impose Negotiation state between the current player and targetPlayer for one turn
  return SUCCESS;
}