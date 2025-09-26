// Author: Gabriel Lagacé

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "../Map/Map.h"

using namespace std;

// ======================== ORDER ======================== //

class Order
{
private:
    // TODO: add initiating Player

    // Unique int identifier
    int *id;

public:
    // Constructors and destructor
    Order(int);
    Order(const Order &other);
    ~Order();

    // Print function and stream overloading function
    virtual void print(std::ostream &) const;
    friend std::ostream &operator<<(ostream &, const Order &);

    /**
     * @brief Accessor method for id
     *
     * @return id
     */
    int getId();

    /**
     * @brief Runs validation checks.
     * Called before the order can be executed.
     *
     * @return false if a check fails, true otherwise
     */
    virtual bool validate();

    /**
     * @brief Executes an order
     *
     * @return 0 on success, 1 on failure
     */
    virtual int execute();
};

// ======================== DEPLOY ======================== //

class Deploy : public Order
{
private:
    int *numTroops;
    Territory *targetTerritory;

public:
    // Constructors and destructor
    Deploy(int, int, Territory *);
    Deploy(const Deploy &other);
    ~Deploy();

    // Print function
    void print(std::ostream &) const override;

    /**
     * @brief Inherited from Order class: runs validation checks.
     * CHECKS
     * numTroops <= # troops in reserve.
     * Player controls targetTerritory.
     *
     * @return false if a check fails, true otherwise
     */
    bool validate() override;

    /**
     * @brief Inherited from Order class: executes the Deploy order.
     * Increases the troops in targetTerritory by numTroops.
     *
     * @return 0 for success, 1 for failure
     */
    int execute() override;
};

// ======================== ADVANCE ======================== //

class Advance : public Order
{
private:
    int *numTroops;
    Territory *sourceTerritory;
    Territory *targetTerritory;

public:
    // Constructor and destructor
    Advance(int, int, Territory *, Territory *);
    Advance(const Advance &other);
    ~Advance();

    // Print function
    void print(std::ostream &) const override;

    /**
     * @brief Inherited from Order class: runs validation checks.
     * CHECKS
     * numTroops <= # troops in reserve.
     * sourceTerritory and targetTerritory are adjacent.
     * Player controls targetTerritory.
     * Controllers of sourceTerritory and targetTerritory must not be in Negotiation state.
     *
     * @return false if a check fails, true otherwise
     */
    bool validate() override;

    /**
     * @brief Inherited from Order class: executes the Advance order.
     * Move troops from sourceTerritory to targetTerritory.
     * Run combat calculations if needed.
     *
     * @return 0 for success, 1 for failure
     */
    int execute() override;
};

// ======================== LA BOMBA ======================== //

class Bomb : public Order
{
private:
    Territory *targetTerritory;

public:
    // Constructor and destructor
    Bomb(int, Territory *);
    Bomb(const Bomb &other);
    ~Bomb();

    // Print function
    void print(std::ostream &) const override;

    /**
     * @brief Inherited from Order class: runs validation checks.
     * CHECKS
     * Player must have a Bomb card in hand.
     * Controller of targetTerritory and the current player must not be in Negotiation state.
     *
     * @return false if a check fails, true otherwise
     */
    bool validate() override;

    /**
     * @brief Inherited from Order class: executes the Bomb order.
     * Halve the number of troops in targetTerritory.
     *
     * @return 0 for success, 1 for failure
     */
    int execute() override;
};

// ======================== BLOCKADE ======================== //

class Blockade : public Order
{
private:
    Territory *targetTerritory;

public:
    // Constructor and destructor
    Blockade(int, Territory *);
    Blockade(const Blockade &other);
    ~Blockade();

    // Print function
    void print(std::ostream &) const override;

    /**
     * @brief Inherited from Order class: runs validation checks.
     * CHECKS
     * Player must have a Blockade card in hand.
     *
     * @return false if a check fails, true otherwise
     */
    bool validate() override;

    /**
     * @brief Inherited from Order class: executes the Blockade order.
     * Triple the number of troops in targetTerritory.
     * Set targetTerritory status to neutral.
     *
     * @return 0 for success, 1 for failure
     */
    int execute() override;
};

// ======================== AIRLIFT ======================== //

class Airlift : public Order
{
private:
    int *numTroops;
    Territory *sourceTerritory;
    Territory *targetTerritory;

public:
    // Constructor and destructor
    Airlift(int, int, Territory *, Territory *);
    Airlift(const Airlift &other);
    ~Airlift();

    // Print function
    void print(std::ostream &) const override;

    /**
     * @brief Inherited from Order class: runs validation checks.
     * CHECKS
     * Player must have an Airlift card in hand.
     * numTroops <= # troops in sourceTerritory.
     * Player must control sourceTerritory.
     * Controllers of sourceTerritory and targetTerritory must not be in Negotiation state.
     *
     * @return false if a check fails, true otherwise
     */
    bool validate() override;

    /**
     * @brief Inherited from Order class: executes the Airlift order.
     * Move numTroops from sourceTerritory to targetTerritory.
     * Run combat calculations if needed.
     *
     * @return 0 for success, 1 for failure
     */
    int execute() override;
};

// ======================== NEGOTIATE ======================== //

class Negotiate : public Order
{
private:
    string *targetPlayer; // TODO: Change to player pointer

public:
    // Constructor and destructor
    Negotiate(int, string);
    Negotiate(const Negotiate &other);
    ~Negotiate();

    // Print function
    void print(std::ostream &) const override;

    /**
     * @brief Inherited from Order class: runs validation checks.
     * CHECKS
     * Player must have a Negotiate card in hand.
     *
     * @return false if a check fails, true otherwise
     */
    bool validate() override;

    /**
     * @brief Inherited from Order class: executes the Negotiate order.
     * Impose Negotiation state between the current player and targetPlayer for one turn.
     *
     * @return 0 for success, 1 for failure
     */
    int execute() override;
};

#endif