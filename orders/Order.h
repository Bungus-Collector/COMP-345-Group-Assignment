// Author: Gabriel Lagacé

#ifndef ORDER_H
#define ORDER_H

#include <string>

using namespace std;

// ORDER

class Order
{
private:
    int *id;

public: // TODO: Copy constructor
    Order(int);
    ~Order();
    int getId();
    virtual bool validate();
    virtual int execute();
};

// DEPLOY

class Deploy : public Order
{
private:
    int *numTroops;
    string *targetTerritory; // Subject to change depending on how territories are identified

public:
    Deploy(int, int, string);
    ~Deploy();
    bool validate() override;
    int execute() override;
};

// ADVANCE

class Advance : public Order
{
private:
    int *numTroops;
    string *sourceTerritory;
    string *targetTerritory;

public:
    Advance(int, int, string, string);
    ~Advance();
    bool validate() override;
    int execute() override;
};

// LA BOMBA

class Bomb : public Order
{
private:
    string *targetTerritory;

public:
    Bomb(int, string);
    ~Bomb();
    bool validate() override;
    int execute() override;
};

// BLOCKADE

class Blockade : public Order
{
private:
    string *targetTerritory;

public:
    Blockade(int, string);
    ~Blockade();
    bool validate() override;
    int execute() override;
};

// AIRLIFT

class Airlift : public Order
{
private:
    int *numTroops;
    string *sourceTerritory;
    string *targetTerritory;

public:
    Airlift(int, int, string, string);
    ~Airlift();
    bool validate() override;
    int execute() override;
};

// NEGOTIATE

class Negotiate : public Order
{
private:
    string *targetPlayer;

public:
    Negotiate(int, string);
    ~Negotiate();
    bool validate() override;
    int execute() override;
};

#endif