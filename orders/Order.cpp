// Author: Gabriel Lagacé

#include <iostream>
#include <string>
#include "Order.h"

using namespace std;

// ORDER

Order::Order(int i)
    : id{i}
{
}

Order::~Order()
{
}

int Order::getId()
{
    return id;
}

// DEPLOY

Deploy::Deploy(int i, int n, string t)
    : Order{i},
      numTroops{n},
      targetTerritory{t}
{
}

// ADVANCE

Advance::Advance(int i, int n, string s, string t)
    : Order{i},
      numTroops{n},
      sourceTerritory{s},
      targetTerritory{t}
{
}

// BOMB

Bomb::Bomb(int i, string t)
    : Order{i},
      targetTerritory{t}
{
}

// BLOCKADE

Blockade::Blockade(int i, string t)
    : Order{i},
      targetTerritory{t}
{
}

// AIRLIFT

Airlift::Airlift(int i, int n, string s, string t)
    : Order{i},
      numTroops{n},
      sourceTerritory{s},
      targetTerritory{t}
{
}

// NEGOTIATE

Negotiate::Negotiate(int i, string p)
    : Order{i},
      targetPlayer{p}
{
}