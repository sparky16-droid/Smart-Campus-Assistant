#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <unordered_set>

/**
 * @file Tokenizer.h
 * @brief Natural language text preprocessing, tokenization, and n-gram extraction.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: MODULAR NLP & TEXT PIPELINE
 * - Normalizes campus questions into cleaned word stems.
 * - Extracts bigrams and trigrams for phrase-level matching (e.g. "computer science", "hostel mess").
 */
class TextTokenizer {
private:
    std::unordered_set<std::string> m_stopWords;

public:
    TextTokenizer();
    ~TextTokenizer() = default;

    std::string cleanAndNormalize(const std::string& input) const;
    std::vector<std::string> tokenize(const std::string& text, bool removeStopWords = true) const;
    std::vector<std::string> extractNGrams(const std::vector<std::string>& tokens, int n) const;
};

#endif // TOKENIZER_H
