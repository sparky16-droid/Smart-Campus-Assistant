#ifndef SYNONYM_GRAPH_H
#define SYNONYM_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/**
 * @file SynonymGraph.h
 * @brief Campus semantic dictionary and synonym expansion graph.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: KNOWLEDGE GRAPH & SYNONYM RESOLUTION
 * - Expands vernacular campus terms and colloquial expressions to official taxonomy.
 * - Multi-directional bidirectional lookup sets.
 */
class SynonymGraph {
private:
    std::unordered_map<std::string, std::string> m_synonymToCanonical;
    std::unordered_map<std::string, std::vector<std::string>> m_canonicalToSynonyms;

    void addSynonymGroup(const std::string& canonical, const std::vector<std::string>& synonyms);

public:
    SynonymGraph();
    ~SynonymGraph() = default;

    std::string getCanonicalTerm(const std::string& word) const;
    std::vector<std::string> expandTokens(const std::vector<std::string>& tokens) const;
    bool areSynonymous(const std::string& word1, const std::string& word2) const;
};

#endif // SYNONYM_GRAPH_H
