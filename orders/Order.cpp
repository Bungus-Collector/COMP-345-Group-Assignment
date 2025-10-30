// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Order.h"
#include "OrdersErrorCodes.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

#define ATTACK_KILL_CHANCE 0.6f
#define DEFEND_KILL_CHANCE 0.7f

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
      issuer{new Player(*p)}
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
  delete issuer;
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
  delete issuer;

  this->id = new int(*other.id);
  this->issuer = new Player(*other.issuer);
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
  delete issuer;
  issuer = new Player(*p);
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
      targetTerritory{new Territory(*t)}
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
  delete targetTerritory;
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

int *Deploy::getNumTroops() { return numTroops; }

Territory *Deploy::getTargetTerritory() { return targetTerritory; }

void Deploy::setNumTroops(int *n)
{
  delete numTroops;
  numTroops = n;
}

void Deploy::setTargetTerritory(Territory *t)
{
  delete targetTerritory;
  targetTerritory = new Territory(*t);
}

bool Deploy::validate()
{
  // CHECKS:
  // Player controls targetTerritory
  if (targetTerritory->getOwner()->getName() != getIssuer()->getName())
    return false;

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
      sourceTerritory{new Territory(*s)},
      targetTerritory{new Territory(*t)}
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
  delete sourceTerritory;
  delete targetTerritory;
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
  if (sourceTerritory->getArmies() < numTroops)
    return false;

  // sourceTerritory and targetTerritory are adjacent
  std::vector<Territory *> *adjacentTerritories = sourceTerritory->getAdjacentTerritories();
  bool isAdjacent = false;
  for (size_t i = 0; i < adjacentTerritories->size(); ++i)
  {
    if (adjacentTerritories->at(i)->getName() == targetTerritory->getName())
    {
      isAdjacent = true;
      break;
    }
  }
  if (!isAdjacent)
    return false;

  // Player must control sourceTerritory
  if (targetTerritory->getOwner()->getName() != getIssuer()->getName())
    return false;

  // Controllers of sourceTerritory and targetTerritory must not be in Negotiation state
  std::string targetPlayerName = targetTerritory->getOwner()->getName();
  std::vector<std::string> negotiatingParters = *getIssuer()->getNegotiatingPartners();
  auto it = std::find(negotiatingParters.begin(), negotiatingParters.end(), targetPlayerName);

  if (it != negotiatingParters.end())
    return false;

  return true;
}

int Advance::execute()
{
  // Move troops from sourceTerritory to targetTerritory
  if (targetTerritory->getOwner() == getIssuer()) // No combat
  {
    sourceTerritory->changeNumArmies(-(*numTroops));
    targetTerritory->changeNumArmies(*numTroops);
  }
  else // Combat
  {
    srand(time(0));
    int attackingArmies = *numTroops;
    int defendingArmies = *targetTerritory->getArmies();

    for (int i = 0; i < attackingArmies; i++) // Attackers damage
    {
      if (defendingArmies == 0)
        break;

      float roll = static_cast<float>(rand()) / RAND_MAX;
      if (roll <= ATTACK_KILL_CHANCE)
        defendingArmies--;
    }

    for (int i = 0; i < defendingArmies; i++) // Defenders damage
    {
      if (attackingArmies == 0)
        break;

      float roll = static_cast<float>(rand()) / RAND_MAX;
      if (roll <= DEFEND_KILL_CHANCE)
        attackingArmies--;
    }

    if (defendingArmies == 0 && attackingArmies > 0)
    {
      sourceTerritory->changeNumArmies(-*numTroops);
      targetTerritory->setArmies(new int(0));
      targetTerritory->changeNumArmies(attackingArmies);
      targetTerritory->setOwner(getIssuer());
    }
    else
    {
      sourceTerritory->changeNumArmies(attackingArmies - *numTroops);
      targetTerritory->setArmies(new int(defendingArmies));
    }
  }
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
      targetTerritory{new Territory(*t)}
{
}

Bomb::Bomb(const Bomb &other) : Order(other)
{
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Bomb::~Bomb()
{
  delete targetTerritory;
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

  this->targetTerritory = other.targetTerritory;
  return *this;
}

Territory *Bomb::getTargetTerritory() { return targetTerritory; }

void Bomb::setTargetTerritory(Territory *t)
{
  delete targetTerritory;
  targetTerritory = new Territory(*t);
}

bool Bomb::validate()
{
  // CHECKS:
  // Issuer must not control targetTerritory
  if (targetTerritory->getOwner()->getName() == getIssuer()->getName())
    return false;

  // targetTerritory must be adjacent to a territory controlled by issuer
  std::vector<Territory *> *adjacentTerritories = targetTerritory->getAdjacentTerritories();
  bool isAdjacent = false;
  for (size_t i = 0; i < adjacentTerritories->size(); ++i)
  {
    if (adjacentTerritories->at(i)->getOwner()->getName() == getIssuer()->getName())
    {
      isAdjacent = true;
      break;
    }
  }
  if (!isAdjacent)
    return false;

  // Controller of targetTerritory and the issuer must not be in Negotiation state
  std::string targetPlayerName = targetTerritory->getOwner()->getName();
  std::vector<std::string> negotiatingParters = *getIssuer()->getNegotiatingPartners();
  auto it = std::find(negotiatingParters.begin(), negotiatingParters.end(), targetPlayerName);

  if (it != negotiatingParters.end())
    return false;

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
      targetTerritory{new Territory(*t)}
{
}

Blockade::Blockade(const Blockade &other) : Order(other)
{
  this->targetTerritory = new Territory(*other.targetTerritory);
}

Blockade::~Blockade()
{
  delete targetTerritory;
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

  this->targetTerritory = other.targetTerritory;
  return *this;
}

Territory *Blockade::getTargetTerritory() { return targetTerritory; }

void Blockade::setTargetTerritory(Territory *t)
{
  delete targetTerritory;
  targetTerritory = new Territory(*t);
}

bool Blockade::validate()
{
  // CHECKS:
  // targetTerritory must not be controlled by an enemy player
  if (targetTerritory->getOwner()->getName() == getIssuer()->getName())
  {
    return false;
  }

  return true;
}

int Blockade::execute()
{
  // Triple the number of troops in targetTerritory
  targetTerritory->changeNumArmies(*(targetTerritory->getArmies()) * 3);

  // Set territory to neutral
  targetTerritory->setOwner(nullptr);
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
      sourceTerritory{new Territory(*s)},
      targetTerritory{new Territory(*t)}
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
  delete sourceTerritory;
  delete targetTerritory;
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
  delete sourceTerritory;
  sourceTerritory = new Territory(*t);
}

void Airlift::setTargetTerritory(Territory *t)
{
  delete targetTerritory;
  targetTerritory = new Territory(*t);
}

bool Airlift::validate()
{
  // CHECKS:
  // numtroops <= # troops in sourceTerritory
  if (sourceTerritory->getArmies() < numTroops)
    return false;

  // Player must control sourceTerritory and targetTerritory
  if (sourceTerritory->getOwner()->getName() != getIssuer()->getName())
    return false;
  if (targetTerritory->getOwner()->getName() != getIssuer()->getName())
    return false;

  return true;
}

int Airlift::execute()
{
  // Move numTroops from sourceTerritory to targetTerritory
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
  if (targetPlayer->getName() == getIssuer()->getName())
    return false;

  return true;
}

int Negotiate::execute()
{
  // Impose Negotiation state between the current player and targetPlayer for one turn
  targetPlayer->addNegotiatingPartner(getIssuer()->getName());
  return SUCCESS;
}