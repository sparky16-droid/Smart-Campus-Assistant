#include "cli/CampusCLI.h"
#include <iostream>

/**
 * @file cli_main.cpp
 * @brief Standalone pure C++ entry point for Smart Campus Assistant CLI.
 * 
 * Can be compiled directly with:
 *   g++ -std=c++17 -O2 src/cli_main.cpp src/core/*.cpp src/algorithms/*.cpp src/nlp/*.cpp src/cli/*.cpp -o CampusAssistantCLI.exe
 */
int main() {
    try {
        CampusCLI cli;
        cli.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal Exception occurred in CampusAssistant: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown Fatal Exception occurred in CampusAssistant." << std::endl;
        return 2;
    }
}
