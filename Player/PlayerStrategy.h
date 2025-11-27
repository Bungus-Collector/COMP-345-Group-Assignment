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
protected:
    Player* owner = nullptr;
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
    virtual PlayerStrategy *clone() const = 0;

    void setOwner(Player* p) { owner = p; }
    Player* getOwner() const { return owner; }

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
    PlayerStrategy *clone() const override;
};

/**
 * @class AggressivePlayerStrategy
 * @brief Always reinforces its strongest territory then attacks if able.
 */
class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    AggressivePlayerStrategy() = default;
    ~AggressivePlayerStrategy() override = default;

    // ---- Overridden Strategy Methods ----
    std::vector<Territory *> toDefend(const Player *p) const override;
    std::vector<Territory *> toAttack(const Player *p) const override;

    /**
     * ------------- Agressive Player Strategy Overview -------------
     *
     *     1. Always dumps every available reinforcement into
     *        its strongest territory (highest numArmies).
     *
     *     2. Always advances all available troops in controlled
     *        territories adjacent to strongest territory into
     *        strongest territory.
     *
     *     3. If available, always bombs an adjacent player's
     *        strongest territory.
     *
     *     4. If available, airlifts all troops from a territory
     *        not adjacent to any enemy controlled territories
     *        into its strongest territory.
     *
     *     5. Does not use blockade or negotiate cards.
     *
     *     6. Attacks any adjacent enemy controlled territories
     *        with all territories containing at least one troop.
     *
     *     7. Repeat.
     */
    void issueOrder(Player *p, Deck *d) override;
    std::string getType() const override;
    PlayerStrategy *clone() const override;
};


/**
 * ==================================================
 * Benevolent Player Strategy
 * ==================================================
 */
class BenevolentPlayerStrategy : public PlayerStrategy{
public:
    BenevolentPlayerStrategy() = default; //default constructor
    ~BenevolentPlayerStrategy() override = default; //destructor

    std::vector<Territory*> toDefend(const Player* player) const override;
    std::vector<Territory*> toAttack(const Player* player) const override;
    void issueOrder(Player* player, Deck* deck) override;
    std::string getType() const override;
    PlayerStrategy *clone() const override;
};

/**
 * @class CheaterPlayerStrategy
 * @brief Automatically conquers all adjacent territories each turn.
 */
class CheaterPlayerStrategy : public PlayerStrategy
{
public:
    CheaterPlayerStrategy() = default;
    ~CheaterPlayerStrategy() override = default;

    // ---- Overridden Strategy Methods ----
    std::vector<Territory *> toDefend(const Player *p) const override;
    std::vector<Territory *> toAttack(const Player *p) const override;
    void issueOrder(Player *p, Deck *deck) override;
    std::string getType() const override;
    PlayerStrategy *clone() const override;
};


/**
 * @class NeutralPlayerStrategy
 * @brief Does nothing unless attacked, then becomes an Aggressive player.
 */
class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    NeutralPlayerStrategy() = default;
    ~NeutralPlayerStrategy() override = default;

    void notifyAttacked(std::string name);

    // ---- Overridden Strategy Methods ----
    std::vector<Territory *> toDefend(const Player *p) const override;
    std::vector<Territory *> toAttack(const Player *p) const override;
    void issueOrder(Player *p, Deck *deck) override;
    std::string getType() const override;
    PlayerStrategy *clone() const override;
};