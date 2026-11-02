#include "CampusEntity.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

uint64_t CampusEntity::getCurrentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

CampusEntity::CampusEntity(std::string id, std::string name, std::string code, std::string description)
    : m_id(std::move(id)),
      m_name(std::move(name)),
      m_code(std::move(code)),
      m_description(std::move(description)),
      m_createdAt(getCurrentTimeMs()),
      m_updatedAt(m_createdAt)
{
    if (m_id.empty()) {
        throw std::invalid_argument("CampusEntity: ID cannot be empty");
    }
    if (m_name.empty()) {
        throw std::invalid_argument("CampusEntity: Name cannot be empty");
    }
}

void CampusEntity::touch() {
    m_updatedAt = getCurrentTimeMs();
}

void CampusEntity::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("CampusEntity::setName: Name cannot be empty");
    }
    m_name = name;
    touch();
}

void CampusEntity::setCode(const std::string& code) {
    m_code = code;
    touch();
}

void CampusEntity::setDescription(const std::string& description) {
    m_description = description;
    touch();
}

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return s;
}

std::vector<std::string> CampusEntity::getSearchTokens() const {
    std::vector<std::string> tokens;
    std::string combined = m_name + " " + m_code + " " + m_description;
    std::stringstream ss(combined);
    std::string token;
    while (ss >> token) {
        token.erase(std::remove_if(token.begin(), token.end(), [](unsigned char c) {
            return !std::isalnum(c);
        }), token.end());
        if (!token.empty()) {
            tokens.push_back(toLower(token));
        }
    }
    return tokens;
}

bool CampusEntity::matchesKeyword(const std::string& keyword) const {
    if (keyword.empty()) return true;
    std::string kw = toLower(keyword);
    if (toLower(m_name).find(kw) != std::string::npos) return true;
    if (toLower(m_code).find(kw) != std::string::npos) return true;
    if (toLower(m_description).find(kw) != std::string::npos) return true;
    return false;
}

std::ostream& operator<<(std::ostream& os, const CampusEntity& entity) {
    entity.printSummary(os);
    return os;
}

bool CampusEntity::operator==(const CampusEntity& other) const {
    return m_id == other.m_id;
}

bool CampusEntity::operator!=(const CampusEntity& other) const {
    return !(*this == other);
}
