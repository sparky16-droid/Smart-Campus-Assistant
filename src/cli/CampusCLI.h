#ifndef CAMPUS_CLI_H
#define CAMPUS_CLI_H

#include "../core/CampusDatabase.h"
#include "../algorithms/CampusGraph.h"
#include "../algorithms/DijkstraRouter.h"
#include "../algorithms/AStarRouter.h"
#include "../algorithms/Levenshtein.h"
#include "../algorithms/Trie.h"
#include "../algorithms/InvertedIndex.h"
#include "../nlp/Tokenizer.h"
#include "../nlp/SynonymGraph.h"
#include "../nlp/IntentClassifier.h"
#include <string>

/**
 * @file CampusCLI.h
 * @brief Interactive Terminal Console for Smart Campus Assistant.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS: CONSOLE UI & INTEGRATION
 * - Standalone C++17 terminal application.
 * - Integrates all OOP models, design patterns, search strategies, and graph pathfinding.
 */
class CampusCLI {
private:
    CampusDatabase& m_db;
    CampusGraph m_graph;
    DijkstraRouter m_dijkstra;
    AStarRouter m_astar;
    CampusTrie m_trie;
    InvertedIndex m_index;
    TextTokenizer m_tokenizer;
    SynonymGraph m_synonyms;
    IntentClassifier m_classifier;

    bool m_running;

    void printBanner() const;
    void showMenu() const;
    void handleChatbot();
    void handleNavigation();
    void handleBuildings();
    void handleFacilities();
    void handleDepartments();
    void handleFaculty();
    void handleNotices();
    void handleFaqs();
    void handleOopDemonstration();
    void handleAdminPortal();

public:
    CampusCLI();
    ~CampusCLI() = default;

    void run();
};

#endif // CAMPUS_CLI_H
