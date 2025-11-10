/**
 * Player.h
 * @author Gorden Quach 402643250
 * @date September 29th, 2025
 */

#pragma once

#include <string>
#include <vector>
#include <iosfwd>

// Forward declaration
class Territory;
class Hand;
class OrdersList;
class Order;

/**
 * @class Player
 *
 * @brief player object for the game
 */
class Player
{
public:
    // ---- Constructor and Destructor ----
    Player(); // default constructor

    explicit Player(const std::string &name);

    Player(const Player &other);

    Player &operator=(const Player &other);

    ~Player(); // Destructor

    // Main player methods

    /**
     * Territories to defend
     * @return list of territory to defend
     */
    std::vector<Territory *> toDefend() const;

    /**
     * Territories to attack
     * @return list of territories to attack
     */
    std::vector<Territory *> toAttack() const;

    /**
     * creates and adds an order to ordersList
     *
     */
    void issueOrder();

    // getters and setters for driver
    // ---Name---
    const std::string &getName() const;
    void setName(const std::string &n);

    // ---Territories---
    void addTerritory(Territory *t); // Player does Not own territory lifetime
    std::vector<Territory *> *getTerritories() const;

    // ---Hand---
    void setHand(Hand *h);              // player owns Hand
    Hand *getHand();                    // player getter for Hands
    void setOrdersList(OrdersList *ol); // player owns orderlist
    OrdersList *getOrdersList() const;

    // ---Order related---
    void setNegotiatingPartners(std::vector<std::string> *);
    std::vector<std::string> *getNegotiatingPartners();
    void addNegotiatingPartner(std::string);
    void resetNegotatingPlayers();
    void setGetsCard(bool);
    bool getGetsCard();

    void addReinforcements(int);
    void takeReinforcements(int);
    int getReinforcements();

    // Stream insertion
    friend std::ostream &operator<<(std::ostream &os, const Player &p);

private:
    // all data members of user-defined class needs to be pointer type
    std::string name_;                              // player name
    std::vector<Territory *> *territories_;         // owned territories
    Hand *hand_;                                    // hand of cards
    OrdersList *orders_;                            // list of orders.
    std::vector<std::string> *negotiatingPartners_; // players that cant be attacked by this player this turn
    bool getsCard_;                                 // true if the player successfully conquered a territory last turn
    int reinforcementPool_;                         // the reinforcement pool of armies the player can deploy

    // deep copy helpers
    void copyFrom(const Player &other);
    void destroy();
};
