#pragma once

#include <iostream>
#include <vector>
#include <string>

class Territory;

/**
 * @class Continent
 * @brief Represents a continent, which is a connected subgraph of territories.
 */
class Continent {
public:
    // Constructors and Destructor
    Continent(const std::string& name, int bonus);
    ~Continent();

    // Copy Constructor and Assignment Operator
    Continent(const Continent& other);
    Continent& operator=(const Continent& other);

    // Stream Insertion Operator
    friend std::ostream& operator<<(std::ostream& os, const Continent& continent);

    // Public Methods

    /**
    * Add territory to continent
    *
    * @param territory pointer to territory to be added.
    */
    void addTerritory(Territory* territory);

    /**
    * Returns the name of the continent.
    *
    * @returns name of continent.
    */
    std::string getName() const;

    /**
    * Returns list of territories of the continent.
    *
    * @returns list of pointers of all territories on the continent.
    */
    std::vector<Territory*>* getTerritories() const;

private:
    std::string* name;
    int* bonus;
    std::vector<Territory*>* territories;
};

/**
 * @class Territory
 * @brief Represents a territory (a node in the map graph).
 */
class Territory {
public:
    // Constructors and Destructor
    Territory(int id, const std::string& name, Continent* continent);
    ~Territory();

    // Copy Constructor and Assignment Operator
    Territory(const Territory& other);
    Territory& operator=(const Territory& other);

    // Stream Insertion Operator
    friend std::ostream& operator<<(std::ostream& os, const Territory& territory);

    // Public Methods

    /**
    * Add a edge from this territory to another.
    *
    * @param territory pointer to territory to be added.
    */
    void addAdjacentTerritory(Territory* territory);

    /**
    * Returns the name of the territory.
    *
    * @return name of territory.
    */
    std::string getName() const;

    /**
    * Returns the id of the territory.
    *
    * @return id of territory.
    */
    int getId() const;

    /**
    * Returns the id of the territory.
    *
    * @return id of territory.
    */
    Continent* getContinent() const;

    /**
    * Returns list of of all adjacent territories.
    *
    * @return list of pointers of all adjacent territories.
    */
    std::vector<Territory*>* getAdjacentTerritories() const;

private:
    int* id;
    std::string* name;
    Continent* continent;
    std::vector<Territory*>* adjacentTerritories;

    // To be implemented later
    // int* armies; // Number of armies on the territory
    // Player* owner; 
};


/**
 * @class Map
 * @brief Represents the entire game map as a connected graph.
 */
class Map {
public:
    // Constructor and Destructor
    Map();
    ~Map();

    // Rule of Three: Copy Constructor and Assignment Operator
    Map(const Map& other);
    Map& operator=(const Map& other);

    // Stream Insertion Operator
    friend std::ostream& operator<<(std::ostream& os, const Map& map);

    // Public Methods

    /**
    * Add continent to map.
    *
    * @param continent pointer to be added to map.
    */
    void addContinent(Continent* continent);

    /**
    * Add territory to map.
    *
    * @param territory pointer to be added to map.
    */
    void addTerritory(Territory* territory);

    /**
    * Validate map
    */
    bool validate() const;

    /**
    * Get a territory by its id.
    *
    * @param id of the territory.
    * @return pointer to the territory object.
    */
    Territory* getTerritoryById(int id) const;

    /**
    * Get all territories on the map.
    *
    * @return list of pointers to all territory objects.
    */
    std::vector<Territory*>* getAllTerritories() const;

    /**
    * Get all Continents on the map.
    *
    * @return list of pointers to all Continent objects.
    */
    std::vector<Continent*>* getAllContinents() const;


private:
    std::vector<Continent*>* allContinents;
    std::vector<Territory*>* allTerritories;

    // Validation helper methods
    bool isMapConnected() const;
    bool areContinentsConnected() const;
    bool doesEachTerritoryBelongToOneContinent() const;
    void traverse(Territory* start, std::vector<bool>& visited) const;
    void traverseContinent(Territory* start, std::vector<bool>& visited, const Continent* continent) const;
};

/**
 * @class MapLoader
 * @brief A utility class to load a map from a .map file.
 */
class MapLoader {
public:
    /**
    * Load a map from a .map file
    * 
    * @param filename of .map file to be loaded.
    * @return pointer to a Map if map is valid, else null pointer.
    */
    Map* loadMap(const std::string& filename);
};