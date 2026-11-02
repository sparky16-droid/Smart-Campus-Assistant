#ifndef NOTICE_ENTITY_H
#define NOTICE_ENTITY_H

#include "CampusEntity.h"

enum class NoticePriority {
    Normal = 0,
    High = 1,
    Urgent = 2
};

/**
 * @file NoticeEntity.h
 * @brief Represents an official campus bulletin notice.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Inherits from CampusEntity.
 * - POLYMORPHISM: Overrides getDetails(), calculateRelevance(), serializeJson(), and printSummary().
 * - OPERATOR OVERLOADING: operator< prioritizing Urgent over High over Normal notices, then newest date.
 */
class NoticeEntity : public CampusEntity {
private:
    std::string m_content;
    std::string m_category; // Academic, Event, Exam, Placement, General
    NoticePriority m_priority;
    bool m_isPublished;
    std::string m_authorName;
    uint64_t m_publishDateMs;

public:
    NoticeEntity(std::string id, std::string title, std::string content, std::string category,
                 NoticePriority priority, bool isPublished, std::string authorName,
                 uint64_t publishDateMs = 0);

    virtual ~NoticeEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "Notice"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getContent() const { return m_content; }
    std::string getCategory() const { return m_category; }
    NoticePriority getPriority() const { return m_priority; }
    std::string getPriorityString() const;
    bool isPublished() const { return m_isPublished; }
    std::string getAuthorName() const { return m_authorName; }
    uint64_t getPublishDateMs() const { return m_publishDateMs; }

    // Mutators
    void setPriority(NoticePriority priority);
    void setPublished(bool published);
    void setContent(const std::string& content);

    // Operator overloading for sorting by urgency
    bool operator<(const NoticeEntity& other) const;
};

#endif // NOTICE_ENTITY_H
