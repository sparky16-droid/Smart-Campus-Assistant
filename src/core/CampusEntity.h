#ifndef CAMPUS_ENTITY_H
#define CAMPUS_ENTITY_H

#include "ICampusEntity.h"
#include <chrono>
#include <sstream>

/**
 * @file CampusEntity.h
 * @brief Base abstract entity class implementing core encapsulation and identity.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: ENCAPSULATION & OPERATOR OVERLOADING
 * - Encapsulation: Strict private state (m_id, m_name, m_code, m_description, timestamps)
 *   with controlled, validated public accessor and mutator methods. Invariants are checked at runtime.
 * - Operator Overloading: Overloads operator==, operator!=, and operator<< for stream formatting.
 */
class CampusEntity : public ICampusEntity {
private:
    std::string m_id;
    std::string m_name;
    std::string m_code;
    std::string m_description;
    uint64_t m_createdAt;
    uint64_t m_updatedAt;

    static uint64_t getCurrentTimeMs();

protected:
    // Protected constructor to prevent direct instantiation of base class
    CampusEntity(std::string id, std::string name, std::string code, std::string description);

    // Protected helper to update modification timestamp
    void touch();

public:
    virtual ~CampusEntity() override = default;

    // Encapsulated Getters
    std::string getId() const override { return m_id; }
    std::string getName() const override { return m_name; }
    std::string getCode() const override { return m_code; }
    std::string getDescription() const override { return m_description; }
    uint64_t getCreatedAt() const { return m_createdAt; }
    uint64_t getUpdatedAt() const { return m_updatedAt; }

    // Encapsulated Setters with validation logic
    void setName(const std::string& name);
    void setCode(const std::string& code);
    void setDescription(const std::string& description);

    // Common search token extraction
    std::vector<std::string> getSearchTokens() const override;
    bool matchesKeyword(const std::string& keyword) const override;

    // Stream operator overload
    friend std::ostream& operator<<(std::ostream& os, const CampusEntity& entity);

    // Equality operator overloads
    bool operator==(const CampusEntity& other) const;
    bool operator!=(const CampusEntity& other) const;
};

#endif // CAMPUS_ENTITY_H
