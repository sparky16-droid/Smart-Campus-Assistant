#include "SearchStrategy.h"
#include <algorithm>
#include <cctype>

static std::string toLowerStrSearch(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double ExactMatchStrategy::score(const std::string& query, const ICampusEntity& entity) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerStrSearch(query);
    if (toLowerStrSearch(entity.getName()) == q) return 1.0;
    if (toLowerStrSearch(entity.getCode()) == q) return 1.0;
    return 0.0;
}

double PrefixMatchStrategy::score(const std::string& query, const ICampusEntity& entity) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerStrSearch(query);
    std::string n = toLowerStrSearch(entity.getName());
    std::string c = toLowerStrSearch(entity.getCode());

    if (n.rfind(q, 0) == 0) return 0.90;
    if (c.rfind(q, 0) == 0) return 0.95;
    return 0.0;
}

double KeywordOverlapStrategy::score(const std::string& query, const ICampusEntity& entity) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerStrSearch(query);
    auto tokens = entity.getSearchTokens();
    if (tokens.empty()) return 0.0;

    int matches = 0;
    for (const auto& token : tokens) {
        if (q.find(token) != std::string::npos || token.find(q) != std::string::npos) {
            matches++;
        }
    }
    return std::min(1.0, static_cast<double>(matches) / tokens.size() * 2.0);
}

void CompositeSearchStrategy::addStrategy(std::shared_ptr<ISearchStrategy> strategy, double weight) {
    if (strategy && weight > 0.0) {
        m_strategies.push_back({strategy, weight});
    }
}

double CompositeSearchStrategy::score(const std::string& query, const ICampusEntity& entity) const {
    if (m_strategies.empty() || query.empty()) return 0.0;
    double totalWeight = 0.0;
    double weightedScore = 0.0;

    for (const auto& ws : m_strategies) {
        double s = ws.strategy->score(query, entity);
        weightedScore += s * ws.weight;
        totalWeight += ws.weight;
    }
    return totalWeight > 0.0 ? (weightedScore / totalWeight) : 0.0;
}
