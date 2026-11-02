#include "Trie.h"
#include <algorithm>
#include <cctype>

CampusTrie::CampusTrie()
    : m_root(std::make_shared<TrieNode>())
{
}

static std::string normalizeTrieString(const std::string& str) {
    std::string s;
    s.reserve(str.size());
    for (char c : str) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            s.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
    }
    return s;
}

void CampusTrie::insert(const std::string& word, const std::string& entityId, int weight) {
    std::string w = normalizeTrieString(word);
    if (w.empty()) return;

    auto current = m_root;
    for (char c : w) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = std::make_shared<TrieNode>();
        }
        current = current->children[c];
    }
    current->isEndOfWord = true;
    current->frequency += weight;
    if (!entityId.empty()) {
        current->payloadEntityId = entityId;
    }
}

bool CampusTrie::search(const std::string& word) const {
    std::string w = normalizeTrieString(word);
    if (w.empty()) return false;

    auto current = m_root;
    for (char c : w) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second;
    }
    return current->isEndOfWord;
}

bool CampusTrie::startsWith(const std::string& prefix) const {
    std::string p = normalizeTrieString(prefix);
    if (p.empty()) return true;

    auto current = m_root;
    for (char c : p) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second;
    }
    return true;
}

void CampusTrie::collectWords(const std::shared_ptr<TrieNode>& node, const std::string& prefix,
                              std::vector<std::pair<std::string, int>>& results, size_t maxSuggestions) const
{
    if (!node || results.size() >= maxSuggestions * 2) return;

    if (node->isEndOfWord) {
        results.push_back({prefix, node->frequency});
    }

    for (const auto& kv : node->children) {
        collectWords(kv.second, prefix + kv.first, results, maxSuggestions);
    }
}

std::vector<std::string> CampusTrie::getAutocompleteSuggestions(const std::string& prefix, size_t maxSuggestions) const {
    std::string p = normalizeTrieString(prefix);
    if (p.empty()) return {};

    auto current = m_root;
    for (char c : p) {
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return {};
        }
        current = it->second;
    }

    std::vector<std::pair<std::string, int>> matches;
    collectWords(current, p, matches, maxSuggestions);

    // Sort by frequency descending
    std::sort(matches.begin(), matches.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    std::vector<std::string> suggestions;
    for (size_t i = 0; i < matches.size() && i < maxSuggestions; ++i) {
        suggestions.push_back(matches[i].first);
    }
    return suggestions;
}

void CampusTrie::clear() {
    m_root = std::make_shared<TrieNode>();
}
