#include "FaqEntity.h"
#include <algorithm>
#include <cctype>

FaqEntity::FaqEntity(std::string id, std::string question, std::string answer, std::string category,
                     std::vector<std::string> keywords, int viewCount, int helpfulCount)
    : CampusEntity(std::move(id), question, "", ""),
      m_question(std::move(question)),
      m_answer(std::move(answer)),
      m_category(std::move(category)),
      m_keywords(std::move(keywords)),
      m_viewCount(viewCount),
      m_helpfulCount(helpfulCount)
{
}

void FaqEntity::incrementViewCount() {
    m_viewCount++;
    touch();
}

void FaqEntity::markHelpful() {
    m_helpfulCount++;
    touch();
}

void FaqEntity::addKeyword(const std::string& kw) {
    if (!kw.empty()) {
        m_keywords.push_back(kw);
        touch();
    }
}

std::string FaqEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== FAQ [" << m_category << "]: " << m_question << " ===\n"
       << "Answer:\n" << m_answer << "\n"
       << "Views: " << m_viewCount << " | Helpful: " << m_helpfulCount << "\n"
       << "Keywords: ";
    for (size_t i = 0; i < m_keywords.size(); ++i) {
        ss << m_keywords[i] << (i + 1 < m_keywords.size() ? ", " : "\n");
    }
    return ss.str();
}

static std::string toLowerFaq(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double FaqEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerFaq(query);
    std::string qu = toLowerFaq(m_question);
    std::string a = toLowerFaq(m_answer);
    std::string cat = toLowerFaq(m_category);

    if (qu == q) return 1.0;
    if (qu.find(q) != std::string::npos) return 0.90;
    if (a.find(q) != std::string::npos) return 0.65;
    if (cat.find(q) != std::string::npos) return 0.60;

    for (const auto& kw : m_keywords) {
        if (toLowerFaq(kw).find(q) != std::string::npos || q.find(toLowerFaq(kw)) != std::string::npos) {
            return 0.85;
        }
    }
    return 0.0;
}

std::string FaqEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"question\":\"" << m_question << "\","
       << "\"answer\":\"" << m_answer << "\","
       << "\"category\":\"" << m_category << "\","
       << "\"viewCount\":" << m_viewCount << ","
       << "\"helpfulCount\":" << m_helpfulCount << ","
       << "\"keywords\":[";
    for (size_t i = 0; i < m_keywords.size(); ++i) {
        ss << "\"" << m_keywords[i] << "\"" << (i + 1 < m_keywords.size() ? "," : "");
    }
    ss << "]}";
    return ss.str();
}

void FaqEntity::printSummary(std::ostream& os) const {
    os << "[FAQ - " << m_category << "] Q: " << m_question 
       << " (Views: " << m_viewCount << ")";
}
