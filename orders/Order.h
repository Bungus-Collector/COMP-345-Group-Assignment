// Author: Gabriel Lagacé

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../GameLogs/LoggingObserver.h"

using namespace std;

// ======================== ORDER ======================== //

class Order : public Subject, public ILoggable
{
private:
    // Initiating Player
    Player *issuer;

    // Unique int identifier
    int *id;

public:
    // Constructors and destructor
    Order();
    Order(int, Player *);
    Order(const Order &);
    virtual ~Order();

    // Print function and stream overloading function
    virtual void print(std::ostream &) const;
    friend std::ostream &operator<<(ostream &, const Order &);

    // Overloading assignment operator
    Order &operator=(const Order &);

    // Helper function for logs
    virtual std::string stringToLog() override;

    // Method for polymorphically copying objects
    virtual Order *clone() const = 0;

    // Getter methods
    int getId();
    Player *getIssuer();

    // Setter methods
    void setId(int);
    void setIssuer(Player *);

    /**
     * @brief Runs validation checks.
     * Called before the order can be executed.
     *
     * @return false if a check fails, true otherwise
     */
    virtual bool validate() = 0;

    /**
     * @brief Executes an order
     *
     * @return 0 on success, error code otherwise
     */
    virtual int execute() = 0;
};

// ======================== DEPLOY ======================== //

class Deploy : public Order
{
private:
    int *numTroops;
    Territory *targetTerritory;

public:
    // Constructors and destructor
    Deploy();
    Deploy(int, Player *, int, Territory *);
    Deploy(const Deploy &other);
    ~Deploy();

    // Print function
    void print(std::ostream &) const override;

    // Overloading assignment operator
    Deploy &operator=(const Deploy &);

    // Method for polymorphically copying objects
    Order *clone() const override;

    // Getter methods
    int *getNumTroops();
    Territory *getTargetTerritory();

    // Setter methods
    void setNumTroops(int *);
    void setTargetTerritory(Territory *);

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
     * @return 0 for success, error code otherwise
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
    Advance();
    Advance(int, Player *, int, Territory *, Territory *);
    Advance(const Advance &other);
    ~Advance();

    // Print function
    void print(std::ostream &) const override;

    // Overloading assignment operator
    Advance &operator=(const Advance &);

    // Method for polymorphically copying objects
    Order *clone() const override;

    // Getter methods
    int *getNumTroops();
    Territory *getSourceTerritory();
    Territory *getTargetTerritory();

    // Setter methods
    void setNumTroops(int *);
    void setSourceTerritory(Territory *);
    void setTargetTerritory(Territory *);

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
     * @return 0 for success, error code otherwise
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
    Bomb();
    Bomb(int, Player *, Territory *);
    Bomb(const Bomb &other);
    ~Bomb();

    // Print function
    void print(std::ostream &) const override;

    // Overloading assignment operator
    Bomb &operator=(const Bomb &);

    // Method for polymorphically copying objects
    Order *clone() const override;

    // Getter method
    Territory *getTargetTerritory();

    // Setter method
    void setTargetTerritory(Territory *);

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
     * @return 0 for success, error code otherwise
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
    Blockade();
    Blockade(int, Player *, Territory *);
    Blockade(const Blockade &other);
    ~Blockade();

    // Print function
    void print(std::ostream &) const override;

    // Overloading assignment operator
    Blockade &operator=(const Blockade &);

    // Method for polymorphically copying objects
    Order *clone() const override;

    // Getter method
    Territory *getTargetTerritory();

    // Setter method
    void setTargetTerritory(Territory *);

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
     * @return 0 for success, error code otherwise
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
    Airlift();
    Airlift(int, Player *, int, Territory *, Territory *);
    Airlift(const Airlift &other);
    ~Airlift();

    // Print function
    void print(std::ostream &) const override;

    // Overloading assignment operator
    Airlift &operator=(const Airlift &);

    // Method for polymorphically copying objects
    Order *clone() const override;

    // Getter methods
    int *getNumTroops();
    Territory *getSourceTerritory();
    Territory *getTargetTerritory();

    // Setter methods
    void setNumTroops(int *);
    void setSourceTerritory(Territory *);
    void setTargetTerritory(Territory *);

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
     * @return 0 for success, error code otherwise
     */
    int execute() override;
};

// ======================== NEGOTIATE ======================== //

class Negotiate : public Order
{
private:
    Player *targetPlayer; // TODO: Change to player pointer

public:
    // Constructor and destructor
    Negotiate();
    Negotiate(int, Player *, Player *);
    Negotiate(const Negotiate &other);
    ~Negotiate();

    // Print function
    void print(std::ostream &) const override;

    // Overloading assignment operator
    Negotiate &operator=(const Negotiate &);

    // Method for polymorphically copying objects
    Order *clone() const override;

    // Getter methods
    Player *getTargetPlayer();

    // Setter methods
    void setTargetPlayer(Player *);

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
     * @return 0 for success, error code otherwise
     */
    int execute() override;
};

#endif