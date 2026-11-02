#include "Tokenizer.h"
#include <sstream>
#include <algorithm>
#include <cctype>

TextTokenizer::TextTokenizer() {
    m_stopWords = {
        "is", "the", "a", "an", "in", "on", "at", "to", "for", "of", "and", "or",
        "where", "what", "how", "when", "who", "which", "can", "i", "you", "me",
        "please", "tell", "about", "give", "show", "find", "locate"
    };
}

std::string TextTokenizer::cleanAndNormalize(const std::string& input) const {
    std::string cleaned;
    cleaned.reserve(input.size());
    for (char c : input) {
        if (std::isalnum(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
            cleaned += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        } else {
            cleaned += ' ';
        }
    }
    return cleaned;
}

std::vector<std::string> TextTokenizer::tokenize(const std::string& text, bool removeStopWords) const {
    std::string normalized = cleanAndNormalize(text);
    std::stringstream ss(normalized);
    std::vector<std::string> tokens;
    std::string word;

    while (ss >> word) {
        if (word.length() < 2) continue;
        if (removeStopWords && m_stopWords.find(word) != m_stopWords.end()) {
            continue;
        }
        tokens.push_back(word);
    }
    return tokens;
}

std::vector<std::string> TextTokenizer::extractNGrams(const std::vector<std::string>& tokens, int n) const {
    std::vector<std::string> ngrams;
    if (n <= 1 || tokens.size() < static_cast<size_t>(n)) {
        return ngrams;
    }

    for (size_t i = 0; i + n <= tokens.size(); ++i) {
        std::string phrase;
        for (int j = 0; j < n; ++j) {
            phrase += tokens[i + j];
            if (j + 1 < n) phrase += " ";
        }
        ngrams.push_back(phrase);
    }
    return ngrams;
}
