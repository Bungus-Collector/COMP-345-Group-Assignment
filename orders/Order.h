// Author: Gabriel Lagacé

#ifndef ORDER_H
#define ORDER_H

#include <string>

using namespace std;

// ORDER

class Order
{
private:
    int id;

public:
    Order(int);
    ~Order();
    int getId();
    bool validate();
    bool execute();
};

// DEPLOY

class Deploy : public Order
{
private:
    int numTroops;
    string targetTerritory; // Subject to change depending on how territories are identified

public:
    Deploy(int, int, string);
    ~Deploy();
};

// ADVANCE

class Advance : public Order
{
private:
    int numTroops;
    string sourceTerritory;
    string targetTerritory;

public:
    Advance(int, int, string, string);
    ~Advance();
};

// LA BOMBA

class Bomb : public Order
{
private:
    string targetTerritory;

public:
    Bomb(int, string);
    ~Bomb();
};

// BLOCKADE

class Blockade : public Order
{
private:
    string targetTerritory;

public:
    Blockade(int, string);
    ~Blockade();
};

// AIRLIFT

class Airlift : public Order
{
private:
    int numTroops;
    string sourceTerritory;
    string targetTerritory;

public:
    Airlift(int, int, string, string);
    ~Airlift();
};

// NEGOTIATE

class Negotiate : public Order
{
private:
    string targetPlayer;

public:
    Negotiate(int, string);
    ~Negotiate();
};

#endif