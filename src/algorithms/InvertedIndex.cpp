#include "InvertedIndex.h"
#include <sstream>
#include <algorithm>
#include <cctype>

IndexDocument::IndexDocument(std::string id_, std::string title_, std::string body_)
    : id(std::move(id_)), title(std::move(title_)), body(std::move(body_)),
      totalWords(0), vectorMagnitude(0.0)
{
}

InvertedIndex::InvertedIndex() {
    initStopWords();
}

void InvertedIndex::initStopWords() {
    m_stopWords = {
        "a", "an", "the", "and", "or", "but", "if", "then", "else", "when",
        "at", "by", "for", "with", "about", "against", "between", "into",
        "through", "during", "before", "after", "above", "below", "to",
        "from", "up", "down", "in", "out", "on", "off", "over", "under",
        "again", "further", "then", "once", "here", "there", "where", "why",
        "how", "all", "any", "both", "each", "few", "more", "most", "other",
        "some", "such", "no", "nor", "not", "only", "own", "same", "so",
        "than", "too", "very", "can", "will", "just", "should", "now",
        "is", "am", "are", "was", "were", "be", "been", "being",
        "have", "has", "had", "having", "do", "does", "did", "doing",
        "i", "me", "my", "myself", "we", "our", "ours", "you", "your",
        "he", "him", "his", "she", "her", "it", "its", "they", "them", "what"
    };
}

std::vector<std::string> InvertedIndex::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : text) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            token += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        } else if (!token.empty()) {
            if (token.size() > 1 && m_stopWords.find(token) == m_stopWords.end()) {
                tokens.push_back(token);
            }
            token.clear();
        }
    }
    if (!token.empty() && token.size() > 1 && m_stopWords.find(token) == m_stopWords.end()) {
        tokens.push_back(token);
    }
    return tokens;
}

void InvertedIndex::addDocument(const std::string& id, const std::string& title, const std::string& body) {
    IndexDocument doc(id, title, body);
    std::vector<std::string> tokens = tokenize(title + " " + body);

    doc.totalWords = static_cast<int>(tokens.size());
    for (const auto& t : tokens) {
        doc.termFrequencies[t]++;
        m_index[t][id]++;
    }

    // Precompute rough vector magnitude for cosine normalization
    double sumSq = 0.0;
    for (const auto& kv : doc.termFrequencies) {
        double tf = static_cast<double>(kv.second) / std::max(1, doc.totalWords);
        sumSq += tf * tf;
    }
    doc.vectorMagnitude = std::sqrt(sumSq);

    m_documents.insert_or_assign(id, doc);
}

void InvertedIndex::removeDocument(const std::string& id) {
    auto it = m_documents.find(id);
    if (it == m_documents.end()) return;

    for (const auto& kv : it->second.termFrequencies) {
        auto idxIt = m_index.find(kv.first);
        if (idxIt != m_index.end()) {
            idxIt->second.erase(id);
            if (idxIt->second.empty()) {
                m_index.erase(idxIt);
            }
        }
    }
    m_documents.erase(it);
}

std::vector<InvertedIndex::SearchResult> InvertedIndex::search(const std::string& query, size_t maxResults) const {
    std::vector<SearchResult> results;
    std::vector<std::string> queryTokens = tokenize(query);
    if (queryTokens.empty() || m_documents.empty()) {
        return results;
    }

    const double totalDocs = static_cast<double>(m_documents.size());
    std::unordered_map<std::string, double> docScores;

    for (const auto& term : queryTokens) {
        auto idxIt = m_index.find(term);
        if (idxIt == m_index.end()) continue;

        const auto& postings = idxIt->second;
        double docFreq = static_cast<double>(postings.size());
        // Standard smoothed IDF: log(1 + (N - df + 0.5)/(df + 0.5))
        double idf = std::log(1.0 + (totalDocs - docFreq + 0.5) / (docFreq + 0.5));
        if (idf < 0.1) idf = 0.1;

        for (const auto& post : postings) {
            const std::string& docId = post.first;
            int count = post.second;

            auto docIt = m_documents.find(docId);
            if (docIt == m_documents.end()) continue;

            double tf = static_cast<double>(count) / std::max(1, docIt->second.totalWords);
            // Title boost
            double titleBoost = (docIt->second.title.find(term) != std::string::npos) ? 2.5 : 1.0;

            docScores[docId] += tf * idf * titleBoost;
        }
    }

    for (const auto& kv : docScores) {
        auto docIt = m_documents.find(kv.first);
        if (docIt != m_documents.end()) {
            double normalizedScore = kv.second;
            if (docIt->second.vectorMagnitude > 0.0) {
                normalizedScore /= docIt->second.vectorMagnitude;
            }
            results.push_back({kv.first, docIt->second.title, normalizedScore});
        }
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.tfIdfScore > b.tfIdfScore;
    });

    if (results.size() > maxResults) {
        results.resize(maxResults);
    }

    return results;
}
