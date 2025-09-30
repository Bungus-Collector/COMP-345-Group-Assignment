/**
 * Player.h
 * @author Gorden Quach 402643250
 * @date September 29th, 2025
 */

 /*
    Todo:
        - documentation
 */
#pragma once

#include <string>
#include <vector>
#include <iosfwd>

using namespace std;

// Forward declaration
class Territory;
class Hand;
class OrdersList;

/**
 * @class Player
 * 
 * @brief player object for the game
 */
class Player{
public: 
    // Constructor and Destructor 
    explicit Player(const string& name);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player(); // Destructor

    // Main player methods
    
    /**
     * Territories to defend
     * @return list of territory to defend
     */
    std::vector<Territory*> toDefend() const;

    /**
     * Territories to attack
     * @return list of territories to attack
     */
    std::vector<Territory*> toAttack() const; 

    /**
     * creates and adds an order to ordersList
     * 
     */
    void issueOrder(); 

    // getters and setters for driver
    const std::string& getName() const;
    void addTerritory(Territory* t);        // Player does Not own territory lifetime 
    void setHand(Hand* h);                  // player owns Hand
    void setOrdersList(OrdersList* ol);     // player owns orderlist

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const Player& p);

private:
    // all data members of user-defined class needs to be pointer type
    std::string name_;                       // player name
    vector<Territory*>* territories_;   //owned territories
    Hand* hand_;                        // hand of cards
    OrdersList* orders_;                //list of orders.

    // deep copy helpers
    void copyFrom(const Player& other);
    void destroy();

};
