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