#include "Map.h"
#include "../Player/Player.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <filesystem>

//==================================================================================//
//                                  Continent Class                                 //
//==================================================================================//

Continent::Continent(const std::string& name, int bonus) {
    this->name = new std::string(name);
    this->bonus = new int(bonus);
    this->territories = new std::vector<Territory*>();
    this->owner = nullptr; // Continent owner initially set to nullptr
}

Continent::Continent() {
    this->name = new std::string();
    this->bonus = new int();
    this->territories = new std::vector<Territory*>();
    this->owner = nullptr;
}

Continent::~Continent() {
    delete name;
    delete bonus;
    delete territories;
    delete owner;
    name = nullptr;
    bonus = nullptr;
    territories = nullptr;
    owner = nullptr;
}

Continent::Continent(const Continent& other) {
    this->name = new std::string(*other.name);
    this->bonus = new int(*other.bonus);
    this->territories = new std::vector<Territory*>(*other.territories);
    if (other.owner != nullptr) {
        this->owner = new Player(*other.owner);
    } else {
        this->owner = nullptr;
    }
}

Continent& Continent::operator=(const Continent& other) {
    if (this == &other) {
        return *this;
    }
    delete name;
    delete bonus;
    delete territories;
    delete owner;
    this->name = new std::string(*other.name);
    this->bonus = new int(*other.bonus);
    this->territories = new std::vector<Territory*>(*other.territories);
    this->owner = new Player(*other.owner);
    return *this;
}

void Continent::addTerritory(Territory* territory) {
    this->territories->push_back(territory);
}

std::string Continent::getName() const { return *name; }
int Continent::getBonus() const { return *bonus; }
std::vector<Territory*>* Continent::getTerritories() const { return territories; }

Player* Continent::getOwner() const { return owner; }
void Continent::setOwner(Player* o){
    delete owner;
    owner = o;
}

std::ostream& operator<<(std::ostream& os, const Continent& continent) {
    os << "Continent: " << *continent.name << " (Bonus: " << *continent.bonus << ")";
    return os;
}

//==================================================================================//
//                                  Territory Class                                 //
//==================================================================================//

Territory::Territory(int id, const std::string& name, Continent* continent, int* armies, Player* owner) {
    this->id = new int(id);
    this->name = new std::string(name);
    this->continent = continent;
    this->adjacentTerritories = new std::vector<Territory*>();
    this->armies = new int(*armies);
    this->owner = owner;
}

Territory::Territory() {
    this->id = new int();
    this->name = new std::string();
    this->continent = new Continent();
    this->adjacentTerritories = new std::vector<Territory*>();
    this->armies = new int(*armies);
    this->owner = nullptr;
}

Territory::~Territory() {
    delete id;
    delete name;
    delete adjacentTerritories;
    delete armies;
    delete owner;
    id = nullptr;
    name = nullptr;
    adjacentTerritories = nullptr;
    continent = nullptr;
    armies = nullptr;
    owner = nullptr;
}

Territory::Territory(const Territory& other) {
    this->id = new int(*other.id);
    this->name = new std::string(*other.name);
    this->continent = other.continent;
    this->adjacentTerritories = new std::vector<Territory*>(*other.adjacentTerritories);
    this->armies = new int(*other.armies);
    if (other.owner != nullptr) {
        this->owner = new Player(*other.owner);
    } else {
        this->owner = nullptr;
    }
}

Territory& Territory::operator=(const Territory& other) {
    if (this == &other) {
        return *this;
    }
    delete id;
    delete name;
    delete adjacentTerritories;
    delete armies;
    delete owner;

    this->id = new int(*other.id);
    this->name = new std::string(*other.name);
    this->continent = other.continent;
    this->adjacentTerritories = new std::vector<Territory*>(*other.adjacentTerritories);
    this->armies = new int(*other.armies);
    this->owner = new Player(*other.owner);
    return *this;
}

void Territory::addAdjacentTerritory(Territory* territory) {
    this->adjacentTerritories->push_back(territory);
}

std::string Territory::getName() const { return *name; }
int Territory::getId() const { return *id; }
Continent* Territory::getContinent() const { return continent; }
std::vector<Territory*>* Territory::getAdjacentTerritories() const { return adjacentTerritories; }

int *Territory::getArmies() const { return armies; }

void Territory::setArmies(int *armies){
    delete armies;
    this->armies = new int(*armies);
}

Player* Territory::getOwner() const { return owner; }

void Territory::setOwner(Player* o){
    delete owner;
    owner = o;
}

void Territory::changeNumArmies(int c) { *armies += c; }

std::ostream& operator<<(std::ostream& os, const Territory& territory) {
    os << "Territory " << *territory.id << ": " << *territory.name
        << " in " << territory.continent->getName() << " | Adjacent to: ";
    if (territory.adjacentTerritories->empty()) {
        os << "None";
    }
    else {
        for (size_t i = 0; i < territory.adjacentTerritories->size(); ++i) {
            os << territory.adjacentTerritories->at(i)->getName() << (i == territory.adjacentTerritories->size() - 1 ? "" : ", ");
        }
    }
    return os;
}

//==================================================================================//
//                                     Map Class                                    //
//==================================================================================//

Map::Map() {
    allContinents = new std::vector<Continent*>();
    allTerritories = new std::vector<Territory*>();
}

Map::~Map() {
    for (Continent* c : *allContinents) delete c;
    delete allContinents;
    for (Territory* t : *allTerritories) delete t;
    delete allTerritories;
}

Map::Map(const Map& other) {
    allContinents = new std::vector<Continent*>();
    allTerritories = new std::vector<Territory*>();
    std::map<const Continent*, Continent*> oldToNewContinents;
    for (const Continent* oldContinent : *other.allContinents) {
        Continent* newContinent = new Continent(*oldContinent);
        this->addContinent(newContinent);
        oldToNewContinents[oldContinent] = newContinent;
    }
    std::map<const Territory*, Territory*> oldToNewTerritories;
    for (const Territory* oldTerritory : *other.allTerritories) {
        Continent* newParentContinent = oldToNewContinents.at(oldTerritory->getContinent());
        Territory* newTerritory = new Territory(*oldTerritory);
        newParentContinent->addTerritory(newTerritory);
        this->addTerritory(newTerritory);
        oldToNewTerritories[oldTerritory] = newTerritory;
    }
    for (const Territory* oldTerritory : *other.allTerritories) {
        Territory* newTerritory = oldToNewTerritories.at(oldTerritory);
        for (const Territory* oldAdjacent : *oldTerritory->getAdjacentTerritories()) {
            Territory* newAdjacent = oldToNewTerritories.at(oldAdjacent);
            newTerritory->addAdjacentTerritory(newAdjacent);
        }
    }
}

Map& Map::operator=(const Map& other) {
    if (this == &other) return *this;
    for (Continent* c : *allContinents) delete c;
    for (Territory* t : *allTerritories) delete t;
    allContinents->clear();
    allTerritories->clear();
    std::map<const Continent*, Continent*> oldToNewContinents;
    for (const Continent* oldContinent : *other.allContinents) {
        Continent* newContinent = new Continent(*oldContinent);
        this->addContinent(newContinent);
        oldToNewContinents[oldContinent] = newContinent;
    }
    std::map<const Territory*, Territory*> oldToNewTerritories;
    for (const Territory* oldTerritory : *other.allTerritories) {
        Continent* newParentContinent = oldToNewContinents.at(oldTerritory->getContinent());
        Territory* newTerritory = new Territory(*oldTerritory);
        newParentContinent->addTerritory(newTerritory);
        this->addTerritory(newTerritory);
        oldToNewTerritories[oldTerritory] = newTerritory;
    }
    for (const Territory* oldTerritory : *other.allTerritories) {
        Territory* newTerritory = oldToNewTerritories.at(oldTerritory);
        for (const Territory* oldAdjacent : *oldTerritory->getAdjacentTerritories()) {
            Territory* newAdjacent = oldToNewTerritories.at(oldAdjacent);
            newTerritory->addAdjacentTerritory(newAdjacent);
        }
    }
    return *this;
}

void Map::addContinent(Continent* continent) { this->allContinents->push_back(continent); }
void Map::addTerritory(Territory* territory) { this->allTerritories->push_back(territory); }
std::vector<Territory*>* Map::getAllTerritories() const { return allTerritories; }
std::vector<Continent*>* Map::getAllContinents() const { return allContinents; }
Territory* Map::getTerritoryById(int id) const {
    for (Territory* t : *allTerritories) if (t->getId() == id) return t;
    return nullptr;
}
bool Map::validate() const {
    if (!isMapConnected()) {
        std::cerr << "Validation Error: Map is not a connected graph." << std::endl;
        return false;
    }
    if (!areContinentsConnected()) {
        std::cerr << "Validation Error: One or more continents are not connected subgraphs." << std::endl;
        return false;
    }
    if (!doesEachTerritoryBelongToOneContinent()) {
        std::cerr << "Validation Error: A territory belongs to more than one continent or none." << std::endl;
        return false;
    }
    return true;
}
bool Map::isMapConnected() const {
    if (allTerritories->empty()) return true;
    std::vector<bool> visited(allTerritories->size() + 1, false);
    traverse(allTerritories->at(0), visited);
    for (Territory* t : *allTerritories) if (!visited[t->getId()]) return false;
    return true;
}
bool Map::areContinentsConnected() const {
    for (Continent* c : *allContinents) {
        std::vector<Territory*>* continentTerritories = c->getTerritories();
        if (continentTerritories->empty()) continue;
        std::vector<bool> visited(allTerritories->size() + 1, false);
        traverseContinent(continentTerritories->at(0), visited, c);
        for (Territory* t : *continentTerritories) if (!visited[t->getId()]) return false;
    }
    return true;
}
bool Map::doesEachTerritoryBelongToOneContinent() const {
    std::map<Territory*, int> territoryContinentCount;
    for (Territory* t : *allTerritories) territoryContinentCount[t] = 0;
    for (Continent* c : *allContinents) {
        for (Territory* t : *c->getTerritories()) {
            if (territoryContinentCount.find(t) != territoryContinentCount.end()) territoryContinentCount[t]++;
            else return false;
        }
    }
    for (auto const& [territory, count] : territoryContinentCount) if (count != 1) return false;
    return true;
}
void Map::traverse(Territory* start, std::vector<bool>& visited) const {
    if (visited[start->getId()]) return;
    visited[start->getId()] = true;
    for (Territory* neighbor : *start->getAdjacentTerritories()) traverse(neighbor, visited);
}
void Map::traverseContinent(Territory* start, std::vector<bool>& visited, const Continent* continent) const {
    if (visited[start->getId()]) return;
    visited[start->getId()] = true;
    for (Territory* neighbor : *start->getAdjacentTerritories()) {
        if (neighbor->getContinent() == continent) traverseContinent(neighbor, visited, continent);
    }
}
std::ostream& operator<<(std::ostream& os, const Map& map) {
    os << "--- MAP DETAILS ---" << std::endl;
    if (map.allContinents->empty()) {
        os << "Map is empty." << std::endl;
        return os;
    }
    for (const auto& continent : *map.allContinents) {
        os << *continent << std::endl;
        for (const auto& territory : *continent->getTerritories()) {
            os << "  -> " << *territory << std::endl;
        }
    }
    os << "-------------------" << std::endl;
    return os;
}

//==================================================================================//
//                                  MapLoader Class                                 //
//==================================================================================//

Map* MapLoader::loadMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open map file " << filename << std::endl;
        return nullptr;
    }

    Map* map = new Map();
    std::string line;
    enum class ParseState { None, Continents, Territories };
    ParseState state = ParseState::None;

    std::map<std::string, Continent*> nameToContinentMap;
    std::map<std::string, Territory*> nameToTerritoryMap;
    // Temporary storage for neighbor names since territories might not exist yet when parsed.
    std::map<Territory*, std::vector<std::string>> territoryToNeighborNames;

    int territoryIdCounter = 1;

    while (getline(file, line)) {
        // Skip comments and empty/irrelevant lines
        if (line.empty() || line.find('=') != std::string::npos || line.find('[') == std::string::npos) {
            if (state == ParseState::None && line.find('[') != std::string::npos && line.find("Continents") == std::string::npos)
                continue;
        }

        if (line.find("[Continents]") != std::string::npos) {
            state = ParseState::Continents;
            continue;
        }
        if (line.find("[Territories]") != std::string::npos) {
            state = ParseState::Territories;
            continue;
        }

        switch (state) {
        case ParseState::Continents: {
            std::stringstream ss(line);
            std::string name;
            int bonus;
            char equals;
            if (getline(ss, name, '=') && ss >> bonus) {
                Continent* c = new Continent(name, bonus);
                map->addContinent(c);
                nameToContinentMap[name] = c;
            }
            break;
        }
        case ParseState::Territories: {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string part;
            std::vector<std::string> parts;
            while (getline(ss, part, ',')) {
                parts.push_back(part);
            }

            if (parts.size() < 4) {
                std::cerr << "Error: Malformed territory line: " << line << std::endl;
                delete map; return nullptr;
            }

            std::string territoryName = parts[0];
            std::string continentName = parts[3];

            if (nameToContinentMap.find(continentName) == nameToContinentMap.end()) {
                std::cerr << "Error: Continent '" << continentName << "' not found for territory '" << territoryName << "'" << std::endl;
                delete map; return nullptr;
            }
            Continent* parentContinent = nameToContinentMap[continentName];
            Territory* t = new Territory(territoryIdCounter++, territoryName, parentContinent, new int(0), nullptr);

            map->addTerritory(t);
            parentContinent->addTerritory(t);
            nameToTerritoryMap[territoryName] = t;

            // Store neighbor names for the second pass
            std::vector<std::string> neighborNames;
            for (size_t i = 4; i < parts.size(); ++i) {
                neighborNames.push_back(parts[i]);
            }
            territoryToNeighborNames[t] = neighborNames;
            break;
        }
        case ParseState::None:
            break;
        }
    }

    for (auto const& [territory, neighborNames] : territoryToNeighborNames) {
        for (const auto& neighborName : neighborNames) {
            if (nameToTerritoryMap.find(neighborName) == nameToTerritoryMap.end()) {
                std::cerr << "Error: Neighbor territory '" << neighborName << "' not found for '" << territory->getName() << "'" << std::endl;
                delete map; return nullptr;
            }
            Territory* neighbor = nameToTerritoryMap[neighborName];
            territory->addAdjacentTerritory(neighbor);
        }
    }

    if (state != ParseState::Territories || map->getAllTerritories()->empty()) {
        std::cerr << "Error: Invalid or incomplete map file format for " << filename << std::endl;
        delete map;
        return nullptr;
    }

    return map;
}