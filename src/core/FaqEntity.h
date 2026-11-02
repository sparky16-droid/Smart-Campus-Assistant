#ifndef FAQ_ENTITY_H
#define FAQ_ENTITY_H

#include "CampusEntity.h"
#include <vector>

/**
 * @file FaqEntity.h
 * @brief Represents a campus frequently asked question.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Inherits from CampusEntity.
 * - POLYMORPHISM: Overrides getDetails(), calculateRelevance(), serializeJson(), and printSummary().
 * - ENCAPSULATION: Encapsulates view counts, helpful feedback tallies, and keyword indexing.
 */
class FaqEntity : public CampusEntity {
private:
    std::string m_question;
    std::string m_answer;
    std::string m_category;
    std::vector<std::string> m_keywords;
    int m_viewCount;
    int m_helpfulCount;

public:
    FaqEntity(std::string id, std::string question, std::string answer, std::string category,
              std::vector<std::string> keywords, int viewCount = 0, int helpfulCount = 0);

    virtual ~FaqEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "FAQ"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getQuestion() const { return m_question; }
    std::string getAnswer() const { return m_answer; }
    std::string getCategory() const { return m_category; }
    const std::vector<std::string>& getKeywords() const { return m_keywords; }
    int getViewCount() const { return m_viewCount; }
    int getHelpfulCount() const { return m_helpfulCount; }

    // Mutators
    void incrementViewCount();
    void markHelpful();
    void addKeyword(const std::string& kw);
};

#endif // FAQ_ENTITY_H
