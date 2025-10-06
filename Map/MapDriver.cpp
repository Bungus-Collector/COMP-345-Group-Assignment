#include "Map.h"
#include <iostream>
#include <vector>
#include <string>

// Test function to demonstrate loading and validating various map files.
void testLoadMaps()
{
    MapLoader loader;
    std::vector<std::string> mapFiles = {
        // Valid Maps
        "Resources/Canada.map",
        "Resources/Americas_1792.map",
        // Invalid Maps
        "Resources/disconnected.map",
        "Resources/disconnected_continents.map",
    };

    std::cout << "========================================\n";
    std::cout << "            Map Loader Test             \n";
    std::cout << "========================================\n\n";

    for (const auto &filename : mapFiles)
    {
        std::cout << "--- Attempting to load map: " << filename << " ---\n";
        Map *map = loader.loadMap(filename);

        if (map != nullptr)
        {
            std::cout << "SUCCESS: Map loaded successfully.\n\n";
            std::cout << *map;

            std::cout << "--- Running Validation ---\n";
            if (map->validate())
            {
                std::cout << "SUCCESS: Map is valid.\n";
            }
            else
            {
                std::cout << "FAILURE: Map is invalid.\n";
                delete map;
                map = nullptr;
            }
        }
        else
        {
            std::cout << "FAILURE: Map could not be loaded or was rejected.\n";
        }
        std::cout << "\n----------------------------------------\n\n";
    }
}

// int main() {
//     testLoadMaps();
//     return 0;
// }