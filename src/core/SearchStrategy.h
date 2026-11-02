#ifndef SEARCH_STRATEGY_H
#define SEARCH_STRATEGY_H

#include "ICampusEntity.h"
#include <string>
#include <vector>
#include <memory>

/**
 * @file SearchStrategy.h
 * @brief Strategy pattern implementations for multi-criteria entity searching.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: STRATEGY DESIGN PATTERN & POLYMORPHISM
 * - Encapsulates alternative search algorithms inside interchangeable strategy objects.
 * - Callers can switch search algorithms at runtime (Exact Match, Levenshtein Fuzzy, Keyword Overlap, Composite).
 */
class ISearchStrategy {
public:
    virtual ~ISearchStrategy() = default;
    virtual double score(const std::string& query, const ICampusEntity& entity) const = 0;
    virtual std::string getStrategyName() const = 0;
};

class ExactMatchStrategy : public ISearchStrategy {
public:
    virtual ~ExactMatchStrategy() override = default;
    double score(const std::string& query, const ICampusEntity& entity) const override;
    std::string getStrategyName() const override { return "ExactMatch"; }
};

class PrefixMatchStrategy : public ISearchStrategy {
public:
    virtual ~PrefixMatchStrategy() override = default;
    double score(const std::string& query, const ICampusEntity& entity) const override;
    std::string getStrategyName() const override { return "PrefixMatch"; }
};

class KeywordOverlapStrategy : public ISearchStrategy {
public:
    virtual ~KeywordOverlapStrategy() override = default;
    double score(const std::string& query, const ICampusEntity& entity) const override;
    std::string getStrategyName() const override { return "KeywordOverlap"; }
};

class CompositeSearchStrategy : public ISearchStrategy {
private:
    struct WeightedStrategy {
        std::shared_ptr<ISearchStrategy> strategy;
        double weight;
    };
    std::vector<WeightedStrategy> m_strategies;

public:
    CompositeSearchStrategy() = default;
    virtual ~CompositeSearchStrategy() override = default;

    void addStrategy(std::shared_ptr<ISearchStrategy> strategy, double weight);
    double score(const std::string& query, const ICampusEntity& entity) const override;
    std::string getStrategyName() const override { return "CompositeSearch"; }
};

#endif // SEARCH_STRATEGY_H
