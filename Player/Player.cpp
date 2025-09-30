
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

#include "Map/Map.h"
#include "orders/Order.h"
#include "orders/OrdersList.h"

#include <iostream>
#include <algorithm>


Player::Player(const std::string& name): 
    name_(name),
    territories_(new std::vector<Territory*>()),
    hand_(nullptr),
    orders_(nullptr){}

Player::Player(const Player& other):
    name_(other.name_),
    territories_(nullptr),
    hand_(nullptr),
    orders_(nullptr){
        copyFrom(other);
    }

Player& Player::operator=(const Player& other){
    if (this != &other) {
        destroy();
        name_ = other.name_;
        copyFrom(other);
    }
    return *this;
}

~Player(){
    destroy();
}


/**
 * @param other reference to other obj
 */
void copyFrom(const Player& other) {
    
}