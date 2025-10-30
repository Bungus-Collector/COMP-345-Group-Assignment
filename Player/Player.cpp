
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
#include "../Cards/Cards.h"

#include <iostream>
#include <algorithm>

/**
 * @class Player
 * @brief default constructor for Player 
 * 
 * Attributes:
 *  -name: Unnamed
 *  -Territories: empty list of territories
 *  -hand: empty hand
 *  -orders: new OrdersList
 */
Player::Player():
    name_("Unnamed"),
    territories_(new std::vector<Territory*>()), //empty list of territories
    hand_(new Hand()), // empty hand
    orders_(new OrdersList()), // every player has its own empty list
    negotiatingPartners_(new std::vector<std::string>()), // no negotiating partners
    getsCard_(false) // default false
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
 *  -hand: empty hand
 *  -orders: new ordersList
 */
Player::Player(const std::string& name): 
    name_(name),
    territories_(new std::vector<Territory*>()),
    hand_(new Hand()),
    orders_(new OrdersList()),
    negotiatingPartners_(new std::vector<std::string>()),
    getsCard_(false)
    {
        std::cout << "[Player] Constructor call for Player "<< name_ << "\n";
    }

/**
 * Copy constructor for Player
 * - creates new Player object with same attributes as the one passed
 * (therefore no need to destroy old data)
 * @param other new Player name.
 */
Player::Player(const Player& other):
    name_(other.name_),
    territories_(nullptr),
    hand_(nullptr),
    orders_(nullptr),
    negotiatingPartners_(nullptr),
    getsCard_(nullptr)
    {
        copyFrom(other);
        std::cout << "[Player] Copy-Constructor call for Player for " << name_ << "\n";
    }

/**
 *  Copy-assignment constructor for Player
 *  - (Deep copy) replaces attributes of another Player with the ones from the Player passed
 * @param other Player object reference
 */
Player& Player::operator=(const Player& other){ //***
    std:cout << "[Player] Copy-assignment constructor called for Player: " << this->name_ << endl;
    if (this != &other) {
        destroy(); // Cleans Player attributes for deep copy
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
        hand_ = new Hand(*other.hand_);
    }else{
        hand_ = new Hand(); //default if no hand_
    }

    if (other.orders_){
        orders_ = new OrdersList(*other.orders_);
    }else{
        orders_ = new OrdersList(); //empty list of orders
    }

    if(other.negotiatingPartners_){
        negotiatingPartners_ = new std::vector<std::string>(*other.negotiatingPartners_);
    }else{
        negotiatingPartners_ = new std::vector<std::string>();
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
    delete negotiatingPartners_;
    negotiatingPartners_ = nullptr;
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
    Order* o = new Order(); // ERROR: Order class is pure virtual
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
 * negotiating partners setter
 */
void Player::setNegotiatingPartners(std::vector<std::string>* np){
    if (negotiatingPartners_ != np){
        delete negotiatingPartners_;
        negotiatingPartners_ = np;
    }
}

/**
 * negotiating partners getter
 * @return negotiating partners*
 */
std::vector<std::string>* Player::getNegotiatingPartners(){
    return negotiatingPartners_;
}

/**
 * Adds a negotiating partner which cannot be attacked this turn
 */
void Player::addNegotiatingPartner(string p){
    negotiatingPartners_->push_back(p);
}

/**
 * Resets negotiating partners (called at the beginning of each new turn)
 */
void Player::resetNegotatingPlayers(){
    delete negotiatingPartners_;
    negotiatingPartners_ = new std::vector<std::string>();
}

/**
 * gets card setter
 */
void Player::setGetsCard(bool gc){
    getsCard_ = gc;
}

/**
 * gets card getter
 * @return bool
 */
bool Player::getGetsCard(){
    return getsCard_;
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