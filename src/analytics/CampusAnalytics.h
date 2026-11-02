#ifndef CAMPUS_ANALYTICS_H
#define CAMPUS_ANALYTICS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <numeric>
#include <cmath>
#include <algorithm>

struct QueryLogEntry {
    std::string queryText;
    std::string matchedIntent;
    double confidence;
    bool wasResolved;
    uint64_t timestampMs;

    QueryLogEntry(std::string text, std::string intent, double conf, bool resolved)
        : queryText(std::move(text)), matchedIntent(std::move(intent)),
          confidence(conf), wasResolved(resolved),
          timestampMs(std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count()) {}
};

struct StatisticalSummary {
    double mean;
    double stdDev;
    double median;
    double minVal;
    double maxVal;
    size_t sampleCount;
};

/**
 * @file CampusAnalytics.h
 * @brief Statistical reporting and campus activity analytics in pure C++.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS: NUMERICAL METHODS, ALGORITHMS & ENCAPSULATION
 * - Computes query latency distributions, intent frequencies, and resolution rates.
 * - Provides summary statistics (mean, variance, median) for campus audit evaluation.
 */
class CampusAnalytics {
private:
    std::vector<QueryLogEntry> m_queryLogs;
    std::unordered_map<std::string, int> m_intentFrequencies;
    std::unordered_map<std::string, int> m_buildingSearchCounts;

public:
    CampusAnalytics() = default;
    ~CampusAnalytics() = default;

    void logQuery(const std::string& queryText, const std::string& intent, double confidence, bool wasResolved);
    void recordBuildingLookup(const std::string& buildingCode);

    double getResolutionRate() const;
    double getAverageConfidence() const;

    StatisticalSummary calculateConfidenceStats() const;
    std::vector<std::pair<std::string, int>> getTopIntents(size_t limit = 5) const;
    std::vector<std::pair<std::string, int>> getMostSearchedBuildings(size_t limit = 5) const;

    std::string generateAuditReport() const;
    void clear();
};

#endif // CAMPUS_ANALYTICS_H
