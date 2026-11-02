#include "CampusAnalytics.h"
#include <sstream>
#include <iomanip>

void CampusAnalytics::logQuery(const std::string& queryText, const std::string& intent, double confidence, bool wasResolved) {
    m_queryLogs.emplace_back(queryText, intent, confidence, wasResolved);
    m_intentFrequencies[intent]++;
}

void CampusAnalytics::recordBuildingLookup(const std::string& buildingCode) {
    if (!buildingCode.empty()) {
        m_buildingSearchCounts[buildingCode]++;
    }
}

double CampusAnalytics::getResolutionRate() const {
    if (m_queryLogs.empty()) return 1.0;
    size_t resolved = 0;
    for (const auto& log : m_queryLogs) {
        if (log.wasResolved) resolved++;
    }
    return static_cast<double>(resolved) / m_queryLogs.size();
}

double CampusAnalytics::getAverageConfidence() const {
    if (m_queryLogs.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& log : m_queryLogs) {
        sum += log.confidence;
    }
    return sum / m_queryLogs.size();
}

StatisticalSummary CampusAnalytics::calculateConfidenceStats() const {
    StatisticalSummary summary = {0, 0, 0, 0, 0, m_queryLogs.size()};
    if (m_queryLogs.empty()) return summary;

    std::vector<double> confs;
    confs.reserve(m_queryLogs.size());
    double sum = 0.0;

    for (const auto& log : m_queryLogs) {
        confs.push_back(log.confidence);
        sum += log.confidence;
    }

    summary.mean = sum / confs.size();
    summary.minVal = *std::min_element(confs.begin(), confs.end());
    summary.maxVal = *std::max_element(confs.begin(), confs.end());

    double varianceSum = 0.0;
    for (double val : confs) {
        varianceSum += (val - summary.mean) * (val - summary.mean);
    }
    summary.stdDev = std::sqrt(varianceSum / confs.size());

    std::sort(confs.begin(), confs.end());
    size_t mid = confs.size() / 2;
    summary.median = (confs.size() % 2 == 0) ? (confs[mid - 1] + confs[mid]) / 2.0 : confs[mid];

    return summary;
}

std::vector<std::pair<std::string, int>> CampusAnalytics::getTopIntents(size_t limit) const {
    std::vector<std::pair<std::string, int>> sorted(m_intentFrequencies.begin(), m_intentFrequencies.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    if (sorted.size() > limit) sorted.resize(limit);
    return sorted;
}

std::vector<std::pair<std::string, int>> CampusAnalytics::getMostSearchedBuildings(size_t limit) const {
    std::vector<std::pair<std::string, int>> sorted(m_buildingSearchCounts.begin(), m_buildingSearchCounts.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    if (sorted.size() > limit) sorted.resize(limit);
    return sorted;
}

std::string CampusAnalytics::generateAuditReport() const {
    std::stringstream ss;
    auto stats = calculateConfidenceStats();

    ss << "============================================================\n"
       << "       SMART CAMPUS ANALYTICS & AUDIT PERFORMANCE REPORT    \n"
       << "============================================================\n"
       << " Total Inquiries Processed: " << m_queryLogs.size() << "\n"
       << " Resolution Success Rate:   " << std::fixed << std::setprecision(1) << (getResolutionRate() * 100.0) << "%\n"
       << " Mean Intent Confidence:    " << std::setprecision(2) << stats.mean << "\n"
       << " Confidence Std Deviation:  " << stats.stdDev << "\n"
       << " Median Confidence Score:   " << stats.median << "\n"
       << " Min / Max Confidence:      [" << stats.minVal << ", " << stats.maxVal << "]\n"
       << "------------------------------------------------------------\n"
       << " Top Inquired Intents:\n";

    for (const auto& kv : getTopIntents(5)) {
        ss << "   * " << std::left << std::setw(20) << kv.first << " : " << kv.second << " queries\n";
    }

    ss << " Top Searched Buildings:\n";
    for (const auto& kv : getMostSearchedBuildings(5)) {
        ss << "   * " << std::left << std::setw(20) << kv.first << " : " << kv.second << " lookups\n";
    }
    ss << "============================================================\n";
    return ss.str();
}

void CampusAnalytics::clear() {
    m_queryLogs.clear();
    m_intentFrequencies.clear();
    m_buildingSearchCounts.clear();
}
