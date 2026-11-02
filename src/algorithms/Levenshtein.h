#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <string>
#include <vector>

/**
 * @file Levenshtein.h
 * @brief Dynamic programming algorithms for string similarity and typo tolerance.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: DYNAMIC PROGRAMMING & TEXT ALGORITHMS
 * - Computes Levenshtein minimum edit distance (insertions, deletions, substitutions).
 * - Computes normalized similarity ratio [0.0, 1.0].
 * - Computes Jaro-Winkler similarity metric for prefix-weighted campus name matching.
 */
class StringSimilarity {
public:
    // Minimum edit distance using DP table
    static int levenshteinDistance(const std::string& s1, const std::string& s2);

    // Normalized ratio: 1.0 = identical, 0.0 = completely disjoint
    static double similarityRatio(const std::string& s1, const std::string& s2);

    // Jaro similarity
    static double jaroSimilarity(const std::string& s1, const std::string& s2);

    // Jaro-Winkler with prefix scaling (p = 0.1, max 4 chars)
    static double jaroWinklerSimilarity(const std::string& s1, const std::string& s2, double prefixScale = 0.1);

    // Fuzzy check if needle approximately appears in haystack
    static bool fuzzyContains(const std::string& haystack, const std::string& needle, double threshold = 0.75);
};

#endif // LEVENSHTEIN_H
