#include "Levenshtein.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <cctype>

static std::string toLowerSim(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

int StringSimilarity::levenshteinDistance(const std::string& s1, const std::string& s2) {
    const size_t len1 = s1.size();
    const size_t len2 = s2.size();

    if (len1 == 0) return static_cast<int>(len2);
    if (len2 == 0) return static_cast<int>(len1);

    // Two-row space optimized DP table
    std::vector<int> prevRow(len2 + 1);
    std::vector<int> currRow(len2 + 1);

    for (size_t j = 0; j <= len2; ++j) {
        prevRow[j] = static_cast<int>(j);
    }

    for (size_t i = 1; i <= len1; ++i) {
        currRow[0] = static_cast<int>(i);
        char c1 = std::tolower(static_cast<unsigned char>(s1[i - 1]));

        for (size_t j = 1; j <= len2; ++j) {
            char c2 = std::tolower(static_cast<unsigned char>(s2[j - 1]));
            int cost = (c1 == c2) ? 0 : 1;

            currRow[j] = std::min({
                prevRow[j] + 1,       // deletion
                currRow[j - 1] + 1,   // insertion
                prevRow[j - 1] + cost // substitution
            });
        }
        prevRow = currRow;
    }

    return currRow[len2];
}

double StringSimilarity::similarityRatio(const std::string& s1, const std::string& s2) {
    if (s1.empty() && s2.empty()) return 1.0;
    if (s1.empty() || s2.empty()) return 0.0;

    int dist = levenshteinDistance(s1, s2);
    size_t maxLen = std::max(s1.size(), s2.size());
    return 1.0 - (static_cast<double>(dist) / static_cast<double>(maxLen));
}

double StringSimilarity::jaroSimilarity(const std::string& s1, const std::string& s2) {
    if (s1 == s2) return 1.0;
    if (s1.empty() || s2.empty()) return 0.0;

    int len1 = static_cast<int>(s1.length());
    int len2 = static_cast<int>(s2.length());
    int matchDistance = std::max(len1, len2) / 2 - 1;
    if (matchDistance < 0) matchDistance = 0;

    std::vector<bool> s1Matches(len1, false);
    std::vector<bool> s2Matches(len2, false);

    int matches = 0;
    for (int i = 0; i < len1; ++i) {
        int start = std::max(0, i - matchDistance);
        int end = std::min(i + matchDistance + 1, len2);

        for (int j = start; j < end; ++j) {
            if (s2Matches[j]) continue;
            if (std::tolower(static_cast<unsigned char>(s1[i])) != std::tolower(static_cast<unsigned char>(s2[j]))) continue;

            s1Matches[i] = true;
            s2Matches[j] = true;
            matches++;
            break;
        }
    }

    if (matches == 0) return 0.0;

    double transpositions = 0.0;
    int k = 0;
    for (int i = 0; i < len1; ++i) {
        if (!s1Matches[i]) continue;
        while (!s2Matches[k]) k++;
        if (std::tolower(static_cast<unsigned char>(s1[i])) != std::tolower(static_cast<unsigned char>(s2[k]))) {
            transpositions += 1.0;
        }
        k++;
    }

    return ((static_cast<double>(matches) / len1) +
            (static_cast<double>(matches) / len2) +
            ((matches - transpositions / 2.0) / matches)) / 3.0;
}

double StringSimilarity::jaroWinklerSimilarity(const std::string& s1, const std::string& s2, double prefixScale) {
    double jaro = jaroSimilarity(s1, s2);
    if (jaro < 0.70) return jaro;

    int prefix = 0;
    int maxPrefix = std::min({4, static_cast<int>(s1.length()), static_cast<int>(s2.length())});
    for (int i = 0; i < maxPrefix; ++i) {
        if (std::tolower(static_cast<unsigned char>(s1[i])) == std::tolower(static_cast<unsigned char>(s2[i]))) {
            prefix++;
        } else {
            break;
        }
    }

    return jaro + prefix * prefixScale * (1.0 - jaro);
}

bool StringSimilarity::fuzzyContains(const std::string& haystack, const std::string& needle, double threshold) {
    if (needle.empty()) return true;
    if (haystack.empty()) return false;

    std::string h = toLowerSim(haystack);
    std::string n = toLowerSim(needle);

    if (h.find(n) != std::string::npos) return true;

    // Word-by-word fuzzy comparison
    std::vector<std::string> words;
    std::string word;
    for (char c : h) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            word += c;
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) words.push_back(word);

    for (const auto& w : words) {
        if (similarityRatio(w, n) >= threshold) {
            return true;
        }
    }

    return false;
}
