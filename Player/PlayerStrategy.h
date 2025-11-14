#pragma once

#include <string>
#include <vector>
#include <iosfwd>

// Forward declaration
class Player;
class Territory;
class Deck;

/**
 * @class PlayerStrategy
 *
 * @brief Abstract base class for a Player's strategy (Strategy Pattern)
 *
 * This class defines the interface that all concrete strategies (e.g.,
 * Human, Aggressive, Benevolent) must implement. The Player class will
 * delegate calls for game decisions to an object of this type.
 */
class PlayerStrategy
{
public:
    // ---- Constructor and Destructor ----
    PlayerStrategy() = default;          // default constructor
    virtual ~PlayerStrategy() = default; // virtual destructor


    /**
     * Determines territories to defend
     * @param p The player executing the strategy
     * @return list of territories to defend
     */
    virtual std::vector<Territory *> toDefend(const Player *p) const = 0;

    /**
     * Determines territories to attack
     * @param p The player executing the strategy
     * @return list of territories to attack
     */
    virtual std::vector<Territory *> toAttack(const Player *p) const = 0;

    /**
     * Creates and adds an order to the player's ordersList
     * @param p The player executing the strategy
     * @param deck The game's deck (for drawing cards)
     */
    virtual void issueOrder(Player *p, Deck *deck) = 0;

    /**
     * Gets the name of the strategy type
     * @return The strategy name as a string
     */
    virtual std::string getType() const = 0;

    /**
     * Virtual-constructor (cloning) for deep copying
     * @return deep cloned PlayerStrategy
     */
    virtual PlayerStrategy* clone() const = 0;

    // Stream insertion
    friend std::ostream &operator<<(std::ostream &os, const PlayerStrategy &ps);

protected:
    // --- Copy/Assignment ---
    // Defaulted to allow Player copy/assignment to work
    PlayerStrategy(const PlayerStrategy &other) = default;
    PlayerStrategy &operator=(const PlayerStrategy &other) = default;
};

// --- Concrete Strategy Implementations ---

/**
 * @class HumanPlayerStrategy
 * @brief Requires user interaction for decisions.
 */
class HumanPlayerStrategy : public PlayerStrategy
{
public:
    HumanPlayerStrategy() = default;
    ~HumanPlayerStrategy() override = default;

    // ---- Overridden Strategy Methods ----
    std::vector<Territory *> toDefend(const Player *p) const override;
    std::vector<Territory *> toAttack(const Player *p) const override;
    void issueOrder(Player *p, Deck *deck) override;
    std::string getType() const override;
    PlayerStrategy* clone() const override;
};