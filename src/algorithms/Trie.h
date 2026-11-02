#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    int frequency;
    std::string payloadEntityId;

    TrieNode() : isEndOfWord(false), frequency(0) {}
};

/**
 * @file Trie.h
 * @brief Prefix Tree data structure for high-speed campus search autocomplete.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: ADVANCED DATA STRUCTURES & TREES
 * - Stores campus building codes, facility names, and faculty names.
 * - Provides O(k) prefix matching and autocomplete suggestions where k is query length.
 */
class CampusTrie {
private:
    std::shared_ptr<TrieNode> m_root;

    void collectWords(const std::shared_ptr<TrieNode>& node, const std::string& prefix,
                      std::vector<std::pair<std::string, int>>& results, size_t maxSuggestions) const;

public:
    CampusTrie();
    ~CampusTrie() = default;

    void insert(const std::string& word, const std::string& entityId = "", int weight = 1);
    bool search(const std::string& word) const;
    bool startsWith(const std::string& prefix) const;

    // Autocomplete returns list of (suggestedWord, frequency)
    std::vector<std::string> getAutocompleteSuggestions(const std::string& prefix, size_t maxSuggestions = 5) const;

    void clear();
};

#endif // TRIE_H
