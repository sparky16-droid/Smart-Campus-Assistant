#ifndef ICAMPUS_ENTITY_H
#define ICAMPUS_ENTITY_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

/**
 * @file ICampusEntity.h
 * @brief Abstract Base Interface defining the contract for all campus entities.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: DATA ABSTRACTION & POLYMORPHISM
 * - Data Abstraction: Hides internal implementation and data representation behind pure virtual methods.
 * - Dynamic Polymorphism: Allows heterogeneous collections of campus entities (std::vector<std::shared_ptr<ICampusEntity>>)
 *   to be treated uniformly while invoking runtime-specific behaviors through virtual method tables (vtable).
 */
class ICampusEntity {
public:
    virtual ~ICampusEntity() = default;

    // Pure virtual getters for core entity metadata
    virtual std::string getId() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getCode() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getEntityType() const = 0;

    // Pure virtual business logic methods demonstrating dynamic polymorphism
    virtual std::string getDetails() const = 0;
    virtual double calculateRelevance(const std::string& query) const = 0;
    virtual std::string serializeJson() const = 0;
    virtual void printSummary(std::ostream& os) const = 0;

    // Utility methods
    virtual bool matchesKeyword(const std::string& keyword) const = 0;
    virtual std::vector<std::string> getSearchTokens() const = 0;
};

#endif // ICAMPUS_ENTITY_H
