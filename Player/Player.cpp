
/**
 * Player.cpp
 * @author Gorden Quach 402643250
 * @date September 29th, 2025
 */

  /*
    Todo:
        - documentation
        - complete methods
 */

#include "Player.h"

#include "../Map/Map.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"

#include <iostream>
#include <algorithm>

/**
 * @class Player
 * @brief default constructor for Player 
 * 
 * Attributes:
 *  -name: Unnamed
 *  -Territories: empty list of territories
 *  -hand: nullptr
 *  -orders: new OrdersList
 */
Player::Player():
    name_("Unnamed"),
    territories_(new std::vector<Territory*>()), //empty list of territories
    hand_(nullptr),
    orders_(new OrdersList()) // every player has its own empty list
    {
        std::cout << "[Player] Default constructor called for " << name_ << "\n";
    }

/**
 * @class Player
 * @brief constructor for Player 
 * 
 * Attributes:
 *  -name: name
 *  -Territories: empty list of territories
 *  -hand: nullptr
 *  -orders: new ordersList
 */
Player::Player(const std::string& name): 
    name_(name),
    territories_(new std::vector<Territory*>()),
    hand_(nullptr),
    orders_(new OrdersList())
    {
        std::cout << "[Player] Copy-Assigned Constructor call for Player "<< name_ << "\n";
    }

/**
 * Copy constructor for Player
 */
Player::Player(const Player& other):
    name_(other.name_),
    territories_(nullptr),
    hand_(nullptr),
    orders_(nullptr)
    {
        copyFrom(other); // deep copy ***
        std::cout << "[Player] Copy-Constructor call for Player for " << name_ << "\n";
    }

/**
 * 
 */
Player& Player::operator=(const Player& other){ //***
    if (this != &other) {
        destroy();
        name_ = other.name_;
        copyFrom(other);
    }
    return *this;
}

/**
 * destructor for Player
 */
Player::~Player(){
    destroy();
}


/**
 * Deep copy for container object; shallow copies the territory* entries ***
 * @param other reference to other Player obj
 */
void Player::copyFrom(const Player& other) {
    // shallow copies the territory* entries ***
    territories_ = new std::vector<Territory*>(*other.territories_);

    // clone other attributes if present

    if (other.hand_){
        //hand_ = new Hand(other.hand_); replace when hand class is created
        hand_ = nullptr;
    }else{
        hand_ = nullptr;
    }

    if (other.orders_){
        orders_ = new OrdersList(*other.orders_);
    }else{
        orders_ = new OrdersList(); //empty list of orders
    }
}

/**
 * Destructor for Player
 *  - handles dangling pointers for destructor
 */
void Player::destroy(){
    delete territories_;
    territories_ = nullptr;
    delete hand_;
    hand_ = nullptr;
    delete orders_;
    orders_ = nullptr;
}

//============ Player functions =============
/**
 * sets arbitrary list of territories to be defended
 */
std::vector<Territory*> Player::toDefend() const{ //***
    std::vector<Territory*> tDefended;

    // checks if player has territories, return if none;
    if(!territories_ || territories_->empty()){
        return tDefended; 
    }

    // arbitrary list of territories to be defended (first half)
    const std::size_t half = territories_->size()/2;
    tDefended.insert(tDefended.end(), territories_->begin(), territories_->begin()+half);
    return tDefended;
}

/**
 * sets arbitrary list of territories to be attacked.
 */
std::vector<Territory*> Player::toAttack() const{ //***
    std::vector<Territory*> tAttack;

    // checks if player has territories, return if empty;
    if(!territories_ || territories_->empty()){
        return tAttack;
    }

    // arbitrary list of territories to be attack (second half)
    const std::size_t half = territories_->size()/2;
    tAttack.insert(tAttack.end(), territories_->begin()+ half, territories_->end());
    return tAttack;
}

/**
 * issues new order for the player
 */
void Player::issueOrder() {
    Order* o = new Order();
    int check = orders_->add(o); //checks if succesfully added

    if (check != 0 ){
        std::cerr << "[Player::issueOrder] Failed to add order into order list check = "<< check << "\n";
        delete o;
        o = nullptr; //handles dangling pointer
    }

}

// ===== getters and setters =====

/**
 * name getter
 * @return string reference 
 */
const std::string& Player::getName() const{
    return name_;
}

/**
 * name setter
 */
void Player::setName(const std::string& n){
    name_ = n;
}

/**
 * territory getter
 * @return vector<Territory*>*
 */
std::vector<Territory*>* Player::getTerritories() const {
    return territories_;
}

/**
 *  add territory
 */
void Player::addTerritory(Territory* t){
    if (!territories_){
        territories_ = new std::vector<Territory*>;
    }
    territories_->push_back(t);
}

/**
 * Hand setters
 */
void Player::setHand(Hand* h){
    if (hand_ != h ){
        delete hand_;
        hand_ = h;
    }
}

/**
 * Hand getters
 * @return Hand*
 */
Hand* Player::getHand(){
    return hand_;
}

/**
 * orders list setter
 */
void Player::setOrdersList(OrdersList* ol){
    if (orders_ != ol){
        delete orders_;
        orders_ = ol;
    }
}

/**
 * OrdersList getter
 * @return OrdersList*
 */
OrdersList* Player::getOrdersList() const{
    return orders_;
}

/**
 * ostream for Player
 * @return ostream&
 */
std::ostream& operator<<(std::ostream& os, const Player& p){
    os << "Player {name = " << p.name_ << ", Territories= ";
    os << (p.territories_ ? p.territories_->size() : 0) 
    << ", hand = " << (p.hand_ ? "yes" : "no")
    <<", orders= " << (p.orders_ ? "yes" : "no")
    << "\n";
    return os;
}