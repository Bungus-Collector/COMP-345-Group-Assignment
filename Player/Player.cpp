
/**
 * Player.cpp
 * @author Gorden Quach 402643250
 * @date September 29th, 2025
 */

#include "Player.h"
#include "PlayerStrategy.h"

#include "../Map/Map.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"
#include "../Cards/Cards.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <set>

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
    ps_(new HumanPlayerStrategy()),
    territories_(new std::vector<Territory*>()), //empty list of territories
    hand_(new Hand()), // empty hand
    orders_(new OrdersList()), // every player has its own empty list
    negotiatingPartners_(new std::vector<std::string>()), // no negotiating partners
    reinforcementPool_(0),
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
    ps_(new HumanPlayerStrategy()),
    territories_(new std::vector<Territory*>()),
    hand_(new Hand()),
    orders_(new OrdersList()),
    negotiatingPartners_(new std::vector<std::string>()),
    reinforcementPool_(0),
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
    ps_(nullptr),
    territories_(nullptr),
    hand_(nullptr),
    orders_(nullptr),
    negotiatingPartners_(nullptr),
    reinforcementPool_(0),
    getsCard_(other.getsCard_)
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
        reinforcementPool_ = other.reinforcementPool_;
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
    if (other.ps_) {
        ps_ = other.ps_->clone();
    } else {
        ps_ = new HumanPlayerStrategy();
    }

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
    delete ps_;
    ps_ = nullptr;
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
std::vector<Territory*> Player::toDefend() const{
    if (ps_) {
        return ps_->toDefend(this);
    }
    std::cout << "Player " << name_ << " has no strategy!" << std::endl;
    return std::vector<Territory*>();
}

/**
 * sets arbitrary list of territories to be attacked.
 */
std::vector<Territory*> Player::toAttack() const{
    if (ps_) {
        return ps_->toAttack(this);
    }
    std::cout << "Player " << name_ << " has no strategy!" << std::endl;
    return std::vector<Territory*>();
}

/**
 * issues new orders for the player
 */
void Player::issueOrder(Deck* gameDeck) {
    if (ps_) {
        ps_->issueOrder(this, gameDeck);
    } else {
        std::cout << "Player " << name_ << " has no strategy and cannot issue orders." << std::endl;
    }
}

void Player::issueOrder_old(Deck* gameDeck) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(territories_->begin(), territories_->end(), gen);
    std::vector<Territory *> attackList = toAttack();
    std::vector<Territory *> defendList = toDefend();

    // DEPLOY REINFORCEMENTS TO OWNED TERRITORIES
    int totalReinforcements = getReinforcements();
    int armiesPerTerritory = 0;
    int remaining = 0;
    if(territories_->size() > 1) {
        armiesPerTerritory = static_cast<int>(totalReinforcements / defendList.size());
        remaining = totalReinforcements % defendList.size();
    }
    else if(territories_->size() == 1) {
        armiesPerTerritory = totalReinforcements;
    }

    OrdersList* list = getOrdersList();
    int nextId = list->getOrders()->empty() ? 1 : list->getOrders()->back()->getId() + 1; // the id for the next order

    for (auto* territory : defendList) {
        int totalArmies = armiesPerTerritory;
        if (remaining-- > 0) {
            totalArmies += 1;
        }

        // Issue Deploy orders
        Deploy* deploy = new Deploy(nextId++, this, totalArmies, territory);
        int result = list->add(deploy);
        if (result != 0) {
            std::cerr << "Failed to add Deploy order for " << territory->getName() << "\n";
            delete deploy;
        }
        else {
            std::cout << "\t" << getName() << " - Issue Deploy order of " << totalArmies << " armies to territory " << territory->getName() << "\n";
            takeReinforcements(totalArmies);
        }
    }

    // Choose to issue 1-4 Advance orders for defence and attack
    std::uniform_int_distribution<> distrib(4, 10);

    // A - ADVANCE ORDERS TO DEFEND
    int numOfDefenseOrders = distrib(gen);
    if (!defendList.empty()) {
        for (int i = 0; i < numOfDefenseOrders; i++) {
            std::uniform_int_distribution<int> targetIndices(0, defendList.size() - 1);
            Territory* target = defendList[targetIndices(gen)];

            // Finding possible territories to take armies from
            std::vector<Territory*> possibleSources;
            for (Territory* territory : *target->getAdjacentTerritories()) {
                auto check3 = territory->getArmies();
                if (territory->getOwner() == this && territory->getArmies() >= 2) {
                    possibleSources.push_back(territory);
                }
            }
            if (possibleSources.empty()) continue;

            // Randomly pick source territory
            std::uniform_int_distribution<int> sourceIndices(0, possibleSources.size() - 1);
            Territory* source = possibleSources[sourceIndices(gen)];

            // Randomly choose number of armies to advance
            int maxArmies = source->getArmies() - 1;
            std::uniform_int_distribution<int> transferQty(1, maxArmies);
            int armiesToMove = transferQty(gen);

            // Issue Advance orders
            Advance* advance = new Advance(nextId++, this, armiesToMove, source, target);
            int result = list->add(advance);
            if (result != 0) {
                std::cerr << "Failed to add Advance order to " << target->getName() << "\n";
                delete advance;
            }
            else {
                std::cout << "\t" << getName() << " - Issue Advance order (Defend) of " << armiesToMove << " armies from territory " << source->getName() << " territory " << target->getName() << "\n";
            }
        }
    }

    // B - ADVANCE ORDERS TO ATTACK
    int numOfAttackOrders = distrib(gen);
    if (!attackList.empty()) {
        for (int i = 0; i < numOfAttackOrders; i++) {
            std::uniform_int_distribution<int> sourceIndices(0, attackList.size() - 1);
            Territory* source = attackList[sourceIndices(gen)];
            if (source->getArmies() < 2) continue;

            // Finding adjacent enemy territories
            std::vector<Territory*> possibleTargets;
            for (auto* territory : *source->getAdjacentTerritories()) {
                if (territory->getOwner() != this) {
                    possibleTargets.push_back(territory);
                }
            }
            if (possibleTargets.empty()) continue;

            // Randomly select target territory
            std::uniform_int_distribution<int> targetIndices(0, possibleTargets.size() - 1);
            Territory* target = possibleTargets[targetIndices(gen)];

            // Randomly choose armies to move to target
            int maxArmies = source->getArmies() - 1;
            std::uniform_int_distribution<int> transferQty(1, maxArmies);
            int armiesToMove = transferQty(gen);

            // Issue Advance orders
            Advance* advance = new Advance(nextId++, this, armiesToMove, source, target);
            int result = list->add(advance);
            if (result != 0) {
                std::cerr << "Failed to add Advance order to " << target->getName() << "\n";
                delete advance;
            }
            else {
                std::cout << "\t" << getName() << " - Issue Advance order (Attack) of " << armiesToMove << " armies from territory " << source->getName() << " to territory " << target->getName() << "\n";
            }
        }
    }

    // C - ISSUE CARD ORDER
    Hand* hand = getHand();
    const std::vector<std::unique_ptr<Card>>& availableCards = hand->getCards();
    if (!availableCards.empty()) {
        Card* selectedCard = availableCards[0].get();
        CardType orderType = selectedCard->getType();
        
        std::vector<Territory*>* ownedTerritories = getTerritories();
        std::uniform_int_distribution<> territoryIndex(0, ownedTerritories->size() - 1);
        switch (orderType) {
            case CardType::BombCard:
                {
                    Territory* target = nullptr;

                    // shuffle indices of ownedTerritories so we don't change the actual territories_ variable
                    std::vector<int> indices(ownedTerritories->size());
                    std::iota(indices.begin(), indices.end(), 0);
                    std::shuffle(indices.begin(), indices.end(), gen);

                    for (int i : indices) {
                        Territory* source = (*ownedTerritories)[i];
                        for (Territory* adjacent : *source->getAdjacentTerritories()) {
                            if (adjacent->getOwner() != this && adjacent->getOwner() != nullptr) {
                                target = adjacent;
                                break;
                            }
                        }
                        if (target != nullptr) break;
                    }

                    if (target != nullptr) {
                        class Bomb* bomb = new class Bomb(nextId++, this, target);
                        int result = list->add(bomb);
                        if (result != 0) {
                            std::cerr << "Failed to add Bomb order for target territory " << target->getName() << "\n";
                            delete bomb;
                        }
                        else {
                            std::cout << "\t" << getName() << " - Issue Bomb order on player " << target->getOwner()->getName() << "'s territory " << target->getName() << "\n";
                        }
                    }
                }
                break;
            // case CardType::ReinforcementCard:
            //     break;
            case CardType::BlockadeCard: {
                std::vector<int> indices(ownedTerritories->size());
                std::iota(indices.begin(), indices.end(), 0);
                std::shuffle(indices.begin(), indices.end(), gen);

                Territory* target = (*ownedTerritories)[indices[0]];

                class Blockade* blockade = new class Blockade(nextId++, this, target);
                int result = list->add(blockade);
                if (result != 0) {
                    std::cerr << "Failed to add Blockade order for target territory " << target->getName() << "\n";
                    delete blockade;
                }
                else {
                    std::cout << "\t" << getName() << " - Issue Blockade order on owned territory " << target->getName() << "\n";
                }
            }
                break;
            case CardType::AirliftCard: {
                std::vector<int> indices(ownedTerritories->size());
                std::iota(indices.begin(), indices.end(), 0);
                std::shuffle(indices.begin(), indices.end(), gen);

                Territory* source = (*ownedTerritories)[indices[0]];
                Territory* target = (*ownedTerritories)[indices[indices.size() - 1]];

                int numOfTroops = source->getArmies() - 1;

                class Airlift* airlift = new class Airlift(nextId++, this, numOfTroops, source, target);
                int result = list->add(airlift);
                if (result != 0) {
                    std::cerr << "Failed to add Airlift order from territory " << source->getName() << " to territory " << target->getName() << "\n";
                    delete airlift;
                }
                else {
                    std::cout << "\t" << getName() << " - Issue Airlift order from territory " << source->getName() << " to territory " << target->getName() << "\n";
                }
            }
            break;
                
            case CardType::DiplomacyCard: {
                std::set<Player*> otherPlayers;

                for (Territory* territory : *ownedTerritories) {
                    for (Territory* adjacent : *territory->getAdjacentTerritories()) {
                        Player* owner = adjacent->getOwner();
                        if (owner != nullptr && owner != this) {
                            otherPlayers.insert(owner);
                        }
                    }
                }

                if (!otherPlayers.empty()) {
                    std::vector<Player*> others(otherPlayers.begin(), otherPlayers.end());
                    std::uniform_int_distribution<int> dis(0, others.size() - 1);

                    Player* target = others[dis(gen)];

                    class Negotiate* negotiate = new class Negotiate(nextId++, this, target);
                    int result = list->add(negotiate);
                    if (result != 0) {
                        std::cerr << "Failed to add Negotiate order from player " << getName() << " to player " << target->getName() << "\n";
                        delete negotiate;
                    }
                    else {
                        std::cout << "\t" << getName() << " - Issue Negotiate order from player " << getName() << " to player " << target->getName() << "\n";
                    }
                }
            }
                break;
            default:
                break;
        }
        selectedCard->play(*hand, *gameDeck);
        gameDeck->draw(*hand);
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
 * Strategy setter
 */
void Player::setStrategy(PlayerStrategy* newStrategy) {
    if (ps_ != newStrategy) {
        delete ps_;
        ps_ = newStrategy;
    }
}

/**
 * Strategy getter
 */
PlayerStrategy* Player::getStrategy() const {
    return ps_;
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
 *  remove territory
 */
void Player::removeTerritory(Territory* t){
    if (!territories_){
        territories_ = new std::vector<Territory*>;
    }
    auto it = std::find(territories_->begin(), territories_->end(), t);
    if (it != territories_->end()) {
        territories_->erase(it);
    }
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

void Player::addReinforcements(int qty) {
    reinforcementPool_ += qty;
}
void Player::takeReinforcements(int qty) {
    reinforcementPool_ -= qty;
}
int Player::getReinforcements() {
    return reinforcementPool_;
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