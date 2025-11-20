#include "PlayerStrategy.h"
#include "Player.h"
#include "../map/Map.h"
#include "../orders/Order.h"
#include "../orders/OrdersList.h"
#include "../Cards/Cards.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <set>
#include <algorithm>

//==================================================================================//
//                       PlayerStrategy Class (Base)                                //
//==================================================================================//

std::ostream &operator<<(std::ostream &os, const PlayerStrategy &ps)
{
    os << ps.getType();
    return os;
}

//==================================================================================//
//                       Human Strategy Helper Functions                            //
//==================================================================================//

static void showTerritoryList(const std::vector<Territory *> &territories)
{
    for (size_t i = 0; i < territories.size(); ++i)
    {
        std::cout << "  " << (i + 1) << ": " << territories[i]->getName()
                  << " (Armies: " << territories[i]->getArmies() << ")" << std::endl;
    }
}

static int getIntegerInput(const std::string &prompt, int min, int max)
{
    int choice = -1;
    while (true)
    {
        std::cout << prompt;
        if (!(std::cin >> choice) || choice < min || choice > max)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between "
                      << min << " and " << max << "." << std::endl;
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return choice;
}

static Territory *selectTerritory(const std::vector<Territory *> &list, const std::string &prompt)
{
    if (list.empty())
    {
        std::cout << "No territories available for this action." << std::endl;
        return nullptr;
    }
    showTerritoryList(list);
    int choice = getIntegerInput(prompt, 1, list.size());
    return list[choice - 1];
}

//==================================================================================//
//                     HumanPlayerStrategy Class                                    //
//==================================================================================//

std::vector<Territory *> HumanPlayerStrategy::toDefend(const Player *p) const
{
    return *p->getTerritories();
}

std::vector<Territory *> HumanPlayerStrategy::toAttack(const Player *p) const
{
    std::set<Territory *> attackableSet;

    for (Territory *myTerritory : *p->getTerritories())
    {
        for (Territory *adjacent : *(myTerritory->getAdjacentTerritories()))
        {
            if (adjacent->getOwner() != p)
            {
                attackableSet.insert(adjacent);
            }
        }
    }

    return std::vector<Territory *>(attackableSet.begin(), attackableSet.end());
}

void HumanPlayerStrategy::issueOrder(Player *p, Deck *deck)
{
    std::vector<Territory *> defendList = toDefend(p);
    std::vector<Territory *> attackList = toAttack(p);

    // --- 1. MANDATORY DEPLOYMENT PHASE ---
    int initialReinforcements = p->getReinforcements();
    int remainingReinforcements = initialReinforcements;

    if (remainingReinforcements > 0)
    {
        std::cout << "\n=== Deployment Phase ===" << std::endl;
        std::cout << "You have " << remainingReinforcements << " reinforcements." << std::endl;
        std::cout << "You *must* deploy all armies before issuing other orders." << std::endl;

        while (remainingReinforcements > 0)
        {
            std::cout << "\nRemaining reinforcements: " << remainingReinforcements << std::endl;
            Territory *target = selectTerritory(defendList, "Select territory to deploy to (1-" + std::to_string(defendList.size()) + "): ");
            if (!target)
            {
                std::cout << "No territories to deploy to." << std::endl;
                break;
            }
            int numToDeploy = getIntegerInput("How many armies to deploy? (1-" + std::to_string(remainingReinforcements) + "): ", 1, remainingReinforcements);

            Deploy *order = new Deploy();
            order->setIssuer(p);
            order->setNumTroops(new int(numToDeploy));
            order->setTargetTerritory(target);

            p->getOrdersList()->add(order);
            remainingReinforcements -= numToDeploy;

            std::cout << "Issuing DEPLOY order: " << numToDeploy << " armies to " << target->getName() << "." << std::endl;
        }

        p->takeReinforcements(initialReinforcements);
        std::cout << "\nAll reinforcements deployed." << std::endl;
    }

    // --- 2. MAIN ORDER PHASE (Advance, Play Card) ---
    std::cout << "\n=== Main Order Phase ===" << std::endl;
    bool stillIssuing = true;
    while (stillIssuing)
    {
        // ... (menu setup is the same) ...
        std::cout << "\nWhat would you like to do?" << std::endl;
        std::cout << "  1. Issue Advance Order (Defend)" << std::endl;
        std::cout << "  2. Issue Advance Order (Attack)" << std::endl;
        std::cout << "  3. Play a Card" << std::endl;
        std::cout << "  0. Finish Issuing Orders" << std::endl;

        int choice = getIntegerInput("Enter your choice (0-3): ", 0, 3);

        switch (choice)
        {
        case 0:
            stillIssuing = false;
            break;

        case 1:
        {
            std::cout << "\n--- Issue Advance (Defend) ---" << std::endl;
            Territory *source = selectTerritory(defendList, "Source territory: ");
            if (!source)
                break;
            Territory *target = selectTerritory(defendList, "Destination territory: ");
            if (!target)
                break;
            if (source == target)
            {
                std::cout << "Source and destination cannot be the same. Order cancelled." << std::endl;
                break;
            }
            int maxArmies = source->getArmies();
            int numArmies = getIntegerInput("Enter number of armies to move (0-" + std::to_string(maxArmies) + "): ", 0, maxArmies);

            Advance *order = new Advance();
            order->setIssuer(p);
            order->setNumTroops(new int(numArmies));
            order->setSourceTerritory(source);
            order->setTargetTerritory(target);

            p->getOrdersList()->add(order);
            std::cout << "Issuing ADVANCE (Defend) from " << source->getName() << " to " << target->getName() << "." << std::endl;
            break;
        }
        case 2:
        {
            std::cout << "\n--- Issue Advance (Attack) ---" << std::endl;
            Territory *source = selectTerritory(defendList, "Source territory: ");
            if (!source)
                break;
            Territory *target = selectTerritory(attackList, "Target territory: ");
            if (!target)
                break;
            int maxArmies = source->getArmies();
            int numArmies = getIntegerInput("Enter number of armies to attack with (0-" + std::to_string(maxArmies) + "): ", 0, maxArmies);

            Advance *order = new Advance();
            order->setIssuer(p);
            order->setNumTroops(new int(numArmies));
            order->setSourceTerritory(source);
            order->setTargetTerritory(target);

            p->getOrdersList()->add(order);
            std::cout << "Issuing ADVANCE (Attack) from " << source->getName() << " to " << target->getName() << "." << std::endl;
            break;
        }
        case 3:
        {
            std::cout << "\n--- Play a Card ---" << std::endl;
            Hand *hand = p->getHand();
            const auto &cards = hand->getCards();

            if (cards.empty())
            {
                std::cout << "Your hand is empty." << std::endl;
                break;
            }

            std::cout << "Your hand:" << std::endl;
            for (size_t i = 0; i < cards.size(); ++i)
            {
                std::cout << "  " << (i + 1) << ": " << *cards[i] << std::endl;
            }
            std::cout << "  0: Cancel" << std::endl;

            int cardChoice = getIntegerInput("Select card to play (0-" + std::to_string(cards.size()) + "): ", 0, cards.size());
            if (cardChoice == 0)
                break;

            Card *selectedCard = cards[cardChoice - 1].get();
            CardType type = selectedCard->getType();
            Order *newOrder = nullptr;

            switch (type)
            {
            case CardType::BombCard:
            {
                std::cout << "Select a target for the BOMB (from attack list):" << std::endl;
                Territory *target = selectTerritory(attackList, "Target territory: ");
                if (target)
                {
                    Bomb *order = new Bomb();
                    order->setIssuer(p);
                    order->setTargetTerritory(target);
                    newOrder = order;
                }
                break;
            }
            case CardType::AirliftCard:
            {
                std::cout << "Select one of your territories to move armies *from*:" << std::endl;
                Territory *source = selectTerritory(defendList, "Source territory: ");
                if (!source)
                    break;
                std::cout << "Select one of your territories to move armies *to*:" << std::endl;
                Territory *target = selectTerritory(defendList, "Destination territory: ");
                if (!target)
                    break;
                int numArmies = getIntegerInput("Armies to airlift (max " + std::to_string(source->getArmies()) + "): ", 0, source->getArmies());

                Airlift *order = new Airlift();
                order->setIssuer(p);
                order->setNumTroops(new int(numArmies));
                order->setSourceTerritory(source);
                order->setTargetTerritory(target);
                newOrder = order;
                break;
            }
            case CardType::BlockadeCard:
            {
                std::cout << "Select one of *your* territories to BLOCKADE:" << std::endl;
                Territory *target = selectTerritory(defendList, "Target territory: ");
                if (target)
                {
                    Blockade *order = new Blockade();
                    order->setIssuer(p);
                    order->setTargetTerritory(target);
                    newOrder = order;
                }
                break;
            }
            case CardType::DiplomacyCard:
            {
                std::cout << "Select a territory owned by the player you wish to negotiate with:" << std::endl;
                Territory *targetTerritory = selectTerritory(attackList, "Select target's territory: ");
                if (targetTerritory)
                {
                    Player *targetPlayer = targetTerritory->getOwner(); // Get Player* from Territory
                    if (targetPlayer == p)
                    {
                        std::cout << "You cannot negotiate with yourself." << std::endl;
                    }
                    else
                    {
                        Negotiate *order = new Negotiate();
                        order->setIssuer(p);
                        order->setTargetPlayer(targetPlayer);
                        newOrder = order;
                    }
                }
                break;
            }

            default:
                std::cout << "Unknown card type. Cannot issue order." << std::endl;
                break;
            }

            if (newOrder)
            {
                p->getOrdersList()->add(newOrder);
                selectedCard->play(*hand, *deck);
                std::cout << "Order issued and card played." << std::endl;
            }
            else
            {
                std::cout << "Card play cancelled." << std::endl;
            }

            break;
        }
        }
    }
    std::cout << "\n--- " << p->getName() << "'s orders are complete. ---" << std::endl;
}

std::string HumanPlayerStrategy::getType() const
{
    return "Human Player";
}

PlayerStrategy *HumanPlayerStrategy::clone() const
{
    return new HumanPlayerStrategy(*this);
}

//==================================================================================//
//                     AggressivePlayerStrategy Class                               //
//==================================================================================//

std::vector<Territory *> AggressivePlayerStrategy::toDefend(const Player *p) const
{
    return *p->getTerritories();
}

std::vector<Territory *> AggressivePlayerStrategy::toAttack(const Player *p) const
{
    std::set<Territory *> targets;

    for (Territory *ownedTerritory : *p->getTerritories())
    {
        for (Territory *adjacent : *(ownedTerritory->getAdjacentTerritories()))
        {
            if (adjacent->getOwner() != p)
                targets.insert(adjacent);
        }
    }

    return std::vector<Territory *>(targets.begin(), targets.end());
}

void AggressivePlayerStrategy::issueOrder(Player *p, Deck *d)
{
    if (p->getTerritories()->size() == 0)
        return; // Owns no territories -> return :(

    // Use a list to store "active" territories which have not advanced yet
    std::list<Territory *> activeTerritories;
    for (Territory *t : *p->getTerritories())
    {
        activeTerritories.push_back(t);
    }

    // Get strongest territory
    int topTroopCount = 0;
    Territory *strongestTerritory;
    for (Territory *t : *p->getTerritories())
    {
        if (t->getArmies() >= topTroopCount)
        {
            strongestTerritory = t;
            topTroopCount = strongestTerritory->getArmies();
        }
    }

    int reinforcements = p->getReinforcements();

    Deploy *reinforcementOrder = new Deploy();
    reinforcementOrder->setIssuer(p);
    reinforcementOrder->setNumTroops(new int(reinforcements));
    reinforcementOrder->setTargetTerritory(strongestTerritory);
    p->getOrdersList()->add(reinforcementOrder);

    // Advance troops into strongest adjacent territory if adjacent
    for (Territory *t : *strongestTerritory->getAdjacentTerritories())
    {
        if (t->getOwner() == p)
        {
            Advance *advanceOrder = new Advance();
            advanceOrder->setIssuer(p);
            advanceOrder->setNumTroops(new int(t->getArmies()));
            advanceOrder->setSourceTerritory(t);
            advanceOrder->setTargetTerritory(strongestTerritory);
            p->getOrdersList()->add(advanceOrder);

            // Remove from active territories
            activeTerritories.remove(t);
        }
    }

    // Play cards
    Hand *hand = p->getHand();
    const auto &cards = hand->getCards();

    if (!cards.empty())
    {
        for (size_t i = 0; i < cards.size(); ++i)
        {
            CardType type = cards[i]->getType();
            Order *newOrder = nullptr;

            switch (type)
            {
            case CardType::BombCard:
            {
                Territory *target;

                // Search for an enemy player controlled territory adjacent to strongest
                for (Territory *t : *strongestTerritory->getAdjacentTerritories())
                {
                    if (t->getOwner() != p)
                    {
                        target = t;
                        break;
                    }
                }

                // If no enemy territories adjacent to strongest then expands search
                if (!target)
                {
                    for (Territory *controlledTerritory : *p->getTerritories())
                    {
                        for (Territory *adjacentTerritory : *controlledTerritory->getAdjacentTerritories())
                        {
                            if (adjacentTerritory->getOwner() != p)
                            {
                                target = adjacentTerritory;
                                break;
                            }
                        }
                    }
                }

                // Only case where target is null is if p controls the whole map, but check anyway
                if (target)
                {
                    Bomb *bombOrder = new Bomb();
                    bombOrder->setIssuer(p);
                    bombOrder->setTargetTerritory(target);
                    p->getOrdersList()->add(bombOrder);
                }
                break;
            }
            case CardType::AirliftCard:
            {
                Territory *source;
                bool suitable;

                // Searches for any territories not adjacent to an enemy territory
                for (Territory *controlledTerritory : *p->getTerritories())
                {
                    suitable = true;

                    // Skip territories with no troops
                    if (controlledTerritory->getArmies() == 0)
                        continue;

                    for (Territory *adjacentTerritory : *controlledTerritory->getAdjacentTerritories())
                    {
                        if (adjacentTerritory->getOwner() != p)
                        {
                            suitable = false;
                            break;
                        }
                    }

                    // If has troops and isnt adjacent to an enemy territory
                    if (suitable)
                    {
                        source = controlledTerritory;
                        break;
                    }
                }

                // Saves card if no suitable territories
                if (source)
                {
                    Airlift *airliftOrder = new Airlift();
                    airliftOrder->setIssuer(p);
                    airliftOrder->setNumTroops(new int(source->getArmies()));
                    airliftOrder->setSourceTerritory(source);
                    airliftOrder->setTargetTerritory(strongestTerritory);
                    p->getOrdersList()->add(airliftOrder);

                    // Remove source from active territories
                    activeTerritories.remove(source);
                }
                break;
            }
            case CardType::BlockadeCard: // Does not blockade
            {
                break;
            }
            case CardType::DiplomacyCard: // Does not negotiate
            {
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }

    // Attacks with all territories with troops and adjacent to an enemy territory
    for (Territory *currentTerritory : activeTerritories)
    {
        // Attempts to invade the weakest enemy controlled adjacent territory
        int weakestTroopCount = std::numeric_limits<int>::max();
        Territory *victim;
        for (Territory *t : *currentTerritory->getAdjacentTerritories())
        {
            if (t->getArmies() <= weakestTroopCount && t->getOwner() != p)
            {
                victim = t;
                weakestTroopCount = victim->getArmies();
            }
        }

        // If has target: obliterate it
        if (victim)
        {
            Advance *advanceOrder = new Advance();
            advanceOrder->setIssuer(p);
            advanceOrder->setNumTroops(new int(currentTerritory->getArmies()));
            advanceOrder->setSourceTerritory(currentTerritory);
            advanceOrder->setIssuer(p);
            p->getOrdersList()->add(advanceOrder);
        }
    }

    std::cout << "\n--- " << p->getName() << "'s orders are complete. ---" << std::endl;
}

std::string AggressivePlayerStrategy::getType() const
{
    return "Aggressive Player";
}

PlayerStrategy *AggressivePlayerStrategy::clone() const
{
    return new AggressivePlayerStrategy(*this);
}

//==================================================================================//
//                     BenevolentPlayerStrategy Class                               //
//==================================================================================//

/**
 * @param p const player
 * @return list of territories
 * @brief will sort the list of territories from weakest to strongest.
 */
std::vector<Territory *> BenevolentPlayerStrategy::toDefend(const Player *p) const
{
    // Copy owned territories
    std::vector<Territory *> territories = *p->getTerritories();

    // Sort by ascending number of armies, weakest first
    std::sort(territories.begin(),territories.end(),[](Territory *a, Territory *b)
        {
            return a->getArmies() < b->getArmies();
        });

    return territories;
}

/** 
 * @param p player const
 * @return list of territories
 * @brief only returns because it doesn't attack
 */
std::vector<Territory *> BenevolentPlayerStrategy::toAttack(const Player* p) const
{
    // Benevolent player never plans to attack
    return {};
}

/**
 * @param p, d player and deck
 * @brief checks if list of territories are empty, then
 * - deploys the reinforcements to its weakest territories
 * - only use defensive cards
 */
void BenevolentPlayerStrategy::issueOrder(Player *p, Deck *d)
{
    if (p->getTerritories()->empty())
        return;

    // Get territories to defend from weakest to strongest
    std::vector<Territory *> defendList = toDefend(p);

    // Use all reinforcements on the weakest territories
    int reinforcements = p->getReinforcements();

    if (reinforcements > 0)
    {
        // Distribute over up to 3 weakest territories
        int numTargets = static_cast<int>(std::min<size_t>(3, defendList.size()));
        int baseAmount = reinforcements / numTargets;
        int extra = reinforcements % numTargets;

        for (int i = 0; i < numTargets; ++i)
        {
            int amount = baseAmount + (i == 0 ? extra : 0); // put remainder on very weakest
            if (amount <= 0){
                continue;
            }

            Deploy *deployOrder = new Deploy();
            deployOrder->setIssuer(p);
            deployOrder->setNumTroops(new int(amount));
            deployOrder->setTargetTerritory(defendList[i]);
            p->getOrdersList()->add(deployOrder);

            std::cout << "[Benevolent] Deploying " << amount << " armies to " << defendList[i]->getName() << ".\n";
        }

        // Mark reinforcements as spent
        p->takeReinforcements(reinforcements);
    }

    // Defensively move troops: from stronger owned territories to weaker ones
    OrdersList *orders = p->getOrdersList();

    for (Territory *weak : defendList)
    {
        // Find a strong friendly neighbor
        Territory *donor = nullptr;
        int donorArmies = 0;

        for (Territory *adj : *weak->getAdjacentTerritories())
        {
            if (adj->getOwner() == p && adj->getArmies() > donorArmies)
            {
                donor = adj;
                donorArmies = adj->getArmies();
            }
        }

        if (donor && donorArmies > weak->getArmies() + 1)
        {
            int moveAmount = donorArmies / 2;
            if (moveAmount <= 0){
                continue;
            }

            Advance *advanceOrder = new Advance();
            advanceOrder->setIssuer(p);
            advanceOrder->setNumTroops(new int(moveAmount));
            advanceOrder->setSourceTerritory(donor);
            advanceOrder->setTargetTerritory(weak);
            orders->add(advanceOrder);

            std::cout << "[Benevolent] Advancing " << moveAmount
                      << " armies from " << donor->getName()
                      << " to " << weak->getName() << ".\n";
        }
    }

    // card options here, only use defensive cards
    Hand* hand = p->getHand();
    auto& cards = hand->getCards();   // same pattern as Human

    if (!cards.empty())
    {
        for (size_t i = 0; i < cards.size(); ++i)
        {
            Card* card = cards[i].get();
            CardType type = card->getType();
            Order* newOrder = nullptr;

            switch (type){

                case CardType::AirliftCard:
                {
                    // Move armies between own territories: from strong to weak
                    if (p->getTerritories()->empty())
                        break;

                    // weakest (first in defendList)
                    Territory* weakest = defendList.front();

                    // find strongest owned territory
                    Territory* strongest = nullptr;
                    int maxArmies = 0;
                    for (Territory* t : *p->getTerritories())
                    {
                        if (t->getArmies() > maxArmies)
                        {
                            strongest = t;
                            maxArmies = t->getArmies();
                        }
                    }

                    if (strongest && strongest != weakest && maxArmies > 1)
                    {
                        int move = maxArmies / 2;

                        Airlift* airlift = new Airlift();
                        airlift->setIssuer(p);
                        airlift->setNumTroops(new int(move));
                        airlift->setSourceTerritory(strongest);
                        airlift->setTargetTerritory(weakest);

                        newOrder = airlift;
                    }
                    break;
                }
                case CardType::BlockadeCard:
                {
                    // Blockade the weakest frontline territory (adjacent to enemies)
                    Territory* target = nullptr;

                    for (Territory* t : defendList)
                    {
                        bool adjacentEnemy = false;
                        for (Territory* adj : *t->getAdjacentTerritories())
                        {
                            if (adj->getOwner() != p)
                            {
                                adjacentEnemy = true;
                                break;
                            }
                        }
                        if (adjacentEnemy)
                        {
                            target = t;
                            break; // first weak frontline we find
                        }
                    }

                    if (target)
                    {
                        Blockade* blockade = new Blockade();
                        blockade->setIssuer(p);
                        blockade->setTargetTerritory(target);
                        newOrder = blockade;
                    }
                    break;
                }
                case CardType::DiplomacyCard:
                {
                    // Negotiate with the owner of an adjacent enemy territory
                    Player* targetPlayer = nullptr;

                    for (Territory* myT : *p->getTerritories())
                    {
                        for (Territory* adj : *myT->getAdjacentTerritories())
                        {
                            if (adj->getOwner() != p)
                            {
                                targetPlayer = adj->getOwner();
                                break;
                            }
                        }
                        if (targetPlayer)
                            break;
                    }

                    if (targetPlayer)
                    {
                        Negotiate* negotiate = new Negotiate();
                        negotiate->setIssuer(p);
                        negotiate->setTargetPlayer(targetPlayer);
                        newOrder = negotiate;
                    }
                    break;
                }
                case CardType::BombCard:
                    // Benevolent NEVER bombs (offensive)
                    break;

                default:
                    break;
            }

            if (newOrder)
            {
                p->getOrdersList()->add(newOrder);
                // this is where Deck actually gets used:
                card->play(*hand, *d);

                std::cout << "[Benevolent] Played card: " << *card << std::endl;
            }
        }
    }

    std::cout << "\n--- " << p->getName() << " (Benevolent) orders are complete. ---\n";
}

/**
 * @return string that shows its type
 */
std::string BenevolentPlayerStrategy::getType() const
{
    return "Benevolent Player";
}

/**
 * @return PlayerStrategy clone object
 */
PlayerStrategy *BenevolentPlayerStrategy::clone() const
{
    return new BenevolentPlayerStrategy(*this);
}

//==================================================================================//
//                        CheaterPlayerStrategy Class                               //
//==================================================================================//

std::vector<Territory *> CheaterPlayerStrategy::toDefend(const Player *p) const
{
    return *p->getTerritories();
}

std::vector<Territory *> CheaterPlayerStrategy::toAttack(const Player *p) const
{
    std::set<Territory *> attackableSet;

    for (Territory *myTerritory : *p->getTerritories())
    {
        for (Territory *adjacent : *(myTerritory->getAdjacentTerritories()))
        {
            if (adjacent->getOwner() != p)
            {
                attackableSet.insert(adjacent);
            }
        }
    }

    return std::vector<Territory *>(attackableSet.begin(), attackableSet.end());
}

void CheaterPlayerStrategy::issueOrder(Player *p, Deck *deck)
{
    std::vector<Territory *> attackList = toAttack(p);

    // Automatically conquer all adjacent enemy territories
    for (Territory *target : attackList)
    {
        Player *defender = target->getOwner();
        target->setOwner(p);
        p->addTerritory(target);
        if(defender != nullptr) {
            defender->removeTerritory(target);
        }

        std::cout << p->getName() << " has magically conquered " << target->getName()
                  << "!" << std::endl;
    }

    std::cout << "\n--- " << p->getName() << "'s orders are complete. ---" << std::endl;
}

std::string CheaterPlayerStrategy::getType() const
{
    return "Cheater Player";
}

PlayerStrategy *CheaterPlayerStrategy::clone() const
{
    return new CheaterPlayerStrategy(*this);
}

//==================================================================================//
//                     NeutralPlayerStrategy Class                               //
//==================================================================================//

std::vector<Territory *> NeutralPlayerStrategy::toDefend(const Player *p) const
{
    return *p->getTerritories();
}

std::vector<Territory *> NeutralPlayerStrategy::toAttack(const Player *p) const
{
    std::set<Territory *> targets;

    for (Territory *ownedTerritory : *p->getTerritories())
    {
        for (Territory *adjacent : *(ownedTerritory->getAdjacentTerritories()))
        {
            if (adjacent->getOwner() != p)
                targets.insert(adjacent);
        }
    }

    return std::vector<Territory *>(targets.begin(), targets.end());
}

void NeutralPlayerStrategy::issueOrder(Player *p, Deck *d)
{
    
    std::cout << "\n--- " << p->getName() << "'s orders are complete. ---" << std::endl;
}

std::string NeutralPlayerStrategy::getType() const
{
    return "Neutral Player";
}

PlayerStrategy *NeutralPlayerStrategy::clone() const
{
    return new NeutralPlayerStrategy(*this);
}