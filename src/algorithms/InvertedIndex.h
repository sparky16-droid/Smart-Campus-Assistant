#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <memory>

struct IndexDocument {
    std::string id;
    std::string title;
    std::string body;
    std::unordered_map<std::string, int> termFrequencies;
    int totalWords;
    double vectorMagnitude;

    IndexDocument(std::string id_, std::string title_, std::string body_);
};

/**
 * @file InvertedIndex.h
 * @brief Inverted Index and TF-IDF Search Engine for Campus Documents and FAQs.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: INFORMATION RETRIEVAL & VECTOR SPACE MODELS
 * - Maps tokens to postings lists.
 * - Computes TF-IDF relevance weights and cosine similarity between user queries and campus documents.
 */
class InvertedIndex {
private:
    std::unordered_map<std::string, IndexDocument> m_documents;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> m_index; // token -> (docId -> count)
    std::unordered_set<std::string> m_stopWords;

    std::vector<std::string> tokenize(const std::string& text) const;
    void initStopWords();

public:
    InvertedIndex();
    ~InvertedIndex() = default;

    void addDocument(const std::string& id, const std::string& title, const std::string& body);
    void removeDocument(const std::string& id);

    struct SearchResult {
        std::string docId;
        std::string title;
        double tfIdfScore;
    };

    std::vector<SearchResult> search(const std::string& query, size_t maxResults = 5) const;
    size_t getDocumentCount() const { return m_documents.size(); }
    size_t getVocabularySize() const { return m_index.size(); }
};

#endif // INVERTED_INDEX_H
