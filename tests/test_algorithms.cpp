#include <iostream>
#include <cassert>
#include "../src/algorithms/CampusGraph.h"
#include "../src/algorithms/DijkstraRouter.h"
#include "../src/algorithms/AStarRouter.h"
#include "../src/algorithms/Levenshtein.h"
#include "../src/algorithms/Trie.h"
#include "../src/algorithms/InvertedIndex.h"

int main() {
    std::cout << "========================================================\n"
              << "   RUNNING C++ ALGORITHMS & DATA STRUCTURES TESTS       \n"
              << "========================================================\n";

    // 1. Test Campus Graph & Dijkstra Shortest Path
    std::cout << "[1/5] Testing Dijkstra Shortest Path Algorithm... ";
    CampusGraph graph;
    DijkstraRouter router(graph);

    auto result = router.findShortestPath("GATE_MAIN", "NODE_LIB");
    assert(result.found);
    assert(!result.pathNodeIds.empty());
    assert(result.totalDistanceMeters > 0.0);
    assert(result.pathNodeIds.front() == "GATE_MAIN");
    assert(result.pathNodeIds.back() == "NODE_LIB");
    std::cout << "PASSED! (Distance: " << result.totalDistanceMeters << "m, Steps: " << result.steps.size() << ")\n";

    // 2. Test A* Heuristic Pathfinding
    std::cout << "[2/5] Testing A* Pathfinding Algorithm... ";
    AStarRouter astar(graph);
    auto astarResult = astar.findPathAStar("GATE_MAIN", "NODE_LIB");
    assert(astarResult.found);
    assert(astarResult.pathNodeIds.front() == "GATE_MAIN");
    assert(astarResult.pathNodeIds.back() == "NODE_LIB");
    assert(std::abs(astarResult.totalDistanceMeters - result.totalDistanceMeters) < 0.001);
    std::cout << "PASSED! (A* Matches Dijkstra distance exactly)\n";

    // 3. Test Levenshtein & Jaro-Winkler
    std::cout << "[3/5] Testing Levenshtein & String Similarity... ";
    assert(StringSimilarity::levenshteinDistance("library", "libary") == 1);
    assert(StringSimilarity::levenshteinDistance("hostel", "hostel") == 0);
    assert(StringSimilarity::similarityRatio("engineering", "engineering") == 1.0);
    assert(StringSimilarity::similarityRatio("cafeteria", "cafetria") > 0.85);
    assert(StringSimilarity::fuzzyContains("Academic Block A", "Block"));
    std::cout << "PASSED!\n";

    // 4. Test Campus Trie
    std::cout << "[4/5] Testing Prefix Tree (Trie) Autocomplete... ";
    CampusTrie trie;
    trie.insert("library", "bld-4", 10);
    trie.insert("librarian", "fac-1", 5);
    trie.insert("laboratory", "fac-3", 8);

    assert(trie.search("library"));
    assert(!trie.search("lib"));
    assert(trie.startsWith("lib"));

    auto suggestions = trie.getAutocompleteSuggestions("lib");
    assert(suggestions.size() >= 2);
    assert(suggestions[0] == "library"); // higher frequency comes first
    std::cout << "PASSED! (Suggestions count: " << suggestions.size() << ")\n";

    // 5. Test Inverted Index & TF-IDF Search
    std::cout << "[5/5] Testing Inverted Index & TF-IDF Scoring... ";
    InvertedIndex index;
    index.addDocument("doc1", "Central Library Timings", "The central library is open Monday to Saturday from 8 AM to 10 PM with reading hall.");
    index.addDocument("doc2", "Campus Cafeteria Menu", "The cafeteria serves fresh vegetarian food, lunch meals, and hot beverages daily.");
    index.addDocument("doc3", "Health Care Clinic", "Emergency medical clinic open 24 hours with doctor consultation and ambulance.");

    auto searchResults = index.search("library books reading");
    assert(!searchResults.empty());
    assert(searchResults[0].docId == "doc1");

    auto foodResults = index.search("cafeteria lunch food");
    assert(!foodResults.empty());
    assert(foodResults[0].docId == "doc2");
    std::cout << "PASSED!\n";

    std::cout << "\n>>> ALL ALGORITHMS AND DATA STRUCTURES TESTS PASSED SUCCESSFULLY! <<<\n";
    return 0;
}
