#include "NoticeEntity.h"
#include <algorithm>
#include <cctype>

NoticeEntity::NoticeEntity(std::string id, std::string title, std::string content, std::string category,
                           NoticePriority priority, bool isPublished, std::string authorName,
                           uint64_t publishDateMs)
    : CampusEntity(std::move(id), std::move(title), "", ""),
      m_content(std::move(content)),
      m_category(std::move(category)),
      m_priority(priority),
      m_isPublished(isPublished),
      m_authorName(std::move(authorName)),
      m_publishDateMs(publishDateMs == 0 ? getCreatedAt() : publishDateMs)
{
}

std::string NoticeEntity::getPriorityString() const {
    switch (m_priority) {
        case NoticePriority::Urgent: return "URGENT";
        case NoticePriority::High:   return "HIGH";
        default:                     return "NORMAL";
    }
}

void NoticeEntity::setPriority(NoticePriority priority) {
    m_priority = priority;
    touch();
}

void NoticeEntity::setPublished(bool published) {
    m_isPublished = published;
    touch();
}

void NoticeEntity::setContent(const std::string& content) {
    m_content = content;
    touch();
}

std::string NoticeEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== NOTICE [" << getPriorityString() << "]: " << getName() << " ===\n"
       << "Category: " << m_category << " | Issued By: " << m_authorName << "\n"
       << "Status: " << (m_isPublished ? "PUBLISHED" : "DRAFT") << "\n"
       << "Content:\n" << m_content << "\n";
    return ss.str();
}

static std::string toLowerNotice(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double NoticeEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerNotice(query);
    std::string t = toLowerNotice(getName());
    std::string c = toLowerNotice(m_content);
    std::string cat = toLowerNotice(m_category);

    if (t == q) return 1.0;
    if (t.find(q) != std::string::npos) return 0.85;
    if (c.find(q) != std::string::npos) return 0.70;
    if (cat.find(q) != std::string::npos) return 0.60;
    return 0.0;
}

std::string NoticeEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"title\":\"" << getName() << "\","
       << "\"content\":\"" << m_content << "\","
       << "\"category\":\"" << m_category << "\","
       << "\"priority\":\"" << getPriorityString() << "\","
       << "\"isPublished\":" << (m_isPublished ? "true" : "false") << ","
       << "\"author\":\"" << m_authorName << "\","
       << "\"publishDate\":" << m_publishDateMs
       << "}";
    return ss.str();
}

void NoticeEntity::printSummary(std::ostream& os) const {
    os << "[" << getPriorityString() << "] " << getName() 
       << " (" << m_category << " - " << m_authorName << ")";
}

bool NoticeEntity::operator<(const NoticeEntity& other) const {
    // Higher priority comes first
    if (static_cast<int>(m_priority) != static_cast<int>(other.m_priority)) {
        return static_cast<int>(m_priority) > static_cast<int>(other.m_priority);
    }
    // Then newer date comes first
    return m_publishDateMs > other.m_publishDateMs;
}
