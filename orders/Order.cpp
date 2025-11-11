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
      issuer{p}
{
}

Order::Order(const Order &other)
{
  this->id = new int(*other.id);
  this->issuer = other.issuer;
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
  if (*numTroops < 0)
    *numTroops = 0;
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

Order *Deploy::clone() const
{
  return new Deploy(*this);
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
  if (!targetTerritory->getOwner())
    return false;

  // CHECKS:
  // Player controls targetTerritory
  if (targetTerritory->getOwner()->getName() != getIssuer()->getName())
    return false;

  return true;
}

int Deploy::execute()
{
  if (!validate())
  {
    std::cout << "[Deploy Order]: " << getIssuer()->getName() << "'s order is invalid!" << std::endl;
    return FAILURE;
  }

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
  if (*numTroops < 0)
    *numTroops = 0;
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

  this->numTroops = new int(*other.numTroops);
  this->sourceTerritory = new Territory(*other.sourceTerritory);
  this->targetTerritory = new Territory(*other.targetTerritory);
  return *this;
}

Order *Advance::clone() const
{
  return new Advance(*this);
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
  sourceTerritory = t;
}

void Advance::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Advance::validate()
{
  if (!sourceTerritory->getOwner() || !targetTerritory->getOwner())
    return false;

  // CHECKS:
  // numtroops <= # troops in sourceTerritory
  if (*sourceTerritory->getArmies() < *numTroops)
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
  if (sourceTerritory->getOwner()->getName() != getIssuer()->getName())
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
  if (!validate())
  {
    std::cout << "[Advance Order]: " << getIssuer()->getName() << "'s order is invalid!" << std::endl;
    return FAILURE;
  }

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
    int attackerCasualties = 0;
    int defendingArmies = *targetTerritory->getArmies();
    int defenderCasualties = 0;

    for (int i = 0; i < attackingArmies; i++) // Attackers damage
    {
      if (defenderCasualties == defendingArmies)
        break;

      float roll = static_cast<float>(rand()) / RAND_MAX;
      if (roll <= ATTACK_KILL_CHANCE)
        defenderCasualties++;
    }

    for (int i = 0; i < defendingArmies; i++) // Defenders damage
    {
      if (attackerCasualties == attackingArmies)
        break;

      float roll = static_cast<float>(rand()) / RAND_MAX;
      if (roll <= DEFEND_KILL_CHANCE)
        attackerCasualties++;
    }

    if (defenderCasualties == defendingArmies && attackerCasualties != attackingArmies)
    {
      sourceTerritory->changeNumArmies(-*numTroops);
      targetTerritory->setArmies(new int(0));
      targetTerritory->changeNumArmies(attackingArmies - attackerCasualties);
      targetTerritory->setOwner(getIssuer());
    }
    else
    {
      sourceTerritory->changeNumArmies(-attackerCasualties);
      targetTerritory->changeNumArmies(-defenderCasualties);
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

Order *Bomb::clone() const
{
  return new Bomb(*this);
}

Territory *Bomb::getTargetTerritory() { return targetTerritory; }

void Bomb::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Bomb::validate()
{
  if (!targetTerritory->getOwner())
    return false;

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
  if (!validate())
  {
    std::cout << "[Bomb Order]: " << getIssuer()->getName() << "'s order is invalid!" << std::endl;
    return FAILURE;
  }

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

Order *Blockade::clone() const
{
  return new Blockade(*this);
}

Territory *Blockade::getTargetTerritory() { return targetTerritory; }

void Blockade::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Blockade::validate()
{
  if (!targetTerritory->getOwner())
    return false;

  // CHECKS:
  // Issuer must control targetTerritory
  if (targetTerritory->getOwner()->getName() != getIssuer()->getName())
  {
    return false;
  }

  return true;
}

int Blockade::execute()
{
  if (!validate())
  {
    std::cout << "[Blockade Order]: " << getIssuer()->getName() << "'s order is invalid!" << std::endl;
    return FAILURE;
  }

  // Triple the number of troops in targetTerritory
  targetTerritory->setArmies(new int(*targetTerritory->getArmies() * 3));

  // Set territory to neutral
  targetTerritory->setOwner(NULL);
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
  if (*numTroops < 0)
    *numTroops = 0;
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

Order *Airlift::clone() const
{
  return new Airlift(*this);
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
  sourceTerritory = t;
}

void Airlift::setTargetTerritory(Territory *t)
{
  targetTerritory = t;
}

bool Airlift::validate()
{
  if (!sourceTerritory->getOwner() || !targetTerritory->getOwner())
    return false;

  // CHECKS:
  // numtroops <= # troops in sourceTerritory

  if (*sourceTerritory->getArmies() < *numTroops)
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
  if (!validate())
  {
    std::cout << "[Airlift Order]: " << getIssuer()->getName() << "'s order is invalid!" << std::endl;
    return FAILURE;
  }

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
      targetPlayer{t}
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

Order *Negotiate::clone() const
{
  return new Negotiate(*this);
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
  if (!validate())
  {
    std::cout << "[Negotiate Order]: " << getIssuer()->getName() << "'s order is invalid!" << std::endl;
    return FAILURE;
  }

  // Impose Negotiation state between the current player and targetPlayer for one turn
  getIssuer()->addNegotiatingPartner(targetPlayer->getName());
  targetPlayer->addNegotiatingPartner(getIssuer()->getName());
  return SUCCESS;
}