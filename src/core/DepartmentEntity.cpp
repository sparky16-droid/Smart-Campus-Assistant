#include "DepartmentEntity.h"
#include <algorithm>
#include <cctype>

DepartmentEntity::DepartmentEntity(std::string id, std::string name, std::string code, std::string description,
                                   std::string buildingId, std::string buildingName, std::string hodName,
                                   std::string contactEmail, std::string contactPhone, std::string websiteUrl,
                                   std::vector<std::string> programs, int studentCount, int facultyCount)
    : CampusEntity(std::move(id), std::move(name), std::move(code), std::move(description)),
      m_buildingId(std::move(buildingId)),
      m_buildingName(std::move(buildingName)),
      m_hodName(std::move(hodName)),
      m_contactEmail(std::move(contactEmail)),
      m_contactPhone(std::move(contactPhone)),
      m_websiteUrl(std::move(websiteUrl)),
      m_programsOffered(std::move(programs)),
      m_studentCount(studentCount),
      m_facultyCount(facultyCount)
{
}

void DepartmentEntity::setHodName(const std::string& hod) {
    m_hodName = hod;
    touch();
}

void DepartmentEntity::addProgram(const std::string& program) {
    if (!program.empty()) {
        m_programsOffered.push_back(program);
        touch();
    }
}

void DepartmentEntity::setCounts(int students, int faculty) {
    m_studentCount = students;
    m_facultyCount = faculty;
    touch();
}

std::string DepartmentEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== DEPARTMENT: " << getName() << " (" << getCode() << ") ===\n"
       << "Head of Department: " << m_hodName << "\n"
       << "Building: " << m_buildingName << "\n"
       << "Contact: " << m_contactEmail << " | " << m_contactPhone << "\n"
       << "Website: " << m_websiteUrl << "\n"
       << "Enrolment: " << m_studentCount << " Students | Faculty: " << m_facultyCount << " Professors\n"
       << "Academic Programs Offered:\n";
    for (const auto& prog : m_programsOffered) {
        ss << "  * " << prog << "\n";
    }
    ss << "Description: " << getDescription() << "\n";
    return ss.str();
}

static std::string toLowerDept(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double DepartmentEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerDept(query);
    std::string n = toLowerDept(getName());
    std::string c = toLowerDept(getCode());
    std::string h = toLowerDept(m_hodName);

    if (n == q || c == q) return 1.0;
    if (n.find(q) != std::string::npos) return 0.85;
    if (c.find(q) != std::string::npos) return 0.90;
    if (h.find(q) != std::string::npos) return 0.70;

    for (const auto& prog : m_programsOffered) {
        if (toLowerDept(prog).find(q) != std::string::npos) return 0.80;
    }
    return 0.0;
}

std::string DepartmentEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"name\":\"" << getName() << "\","
       << "\"code\":\"" << getCode() << "\","
       << "\"description\":\"" << getDescription() << "\","
       << "\"buildingId\":\"" << m_buildingId << "\","
       << "\"buildingName\":\"" << m_buildingName << "\","
       << "\"hodName\":\"" << m_hodName << "\","
       << "\"contactEmail\":\"" << m_contactEmail << "\","
       << "\"contactPhone\":\"" << m_contactPhone << "\","
       << "\"websiteUrl\":\"" << m_websiteUrl << "\","
       << "\"studentCount\":" << m_studentCount << ","
       << "\"facultyCount\":" << m_facultyCount << ","
       << "\"programs\":[";
    for (size_t i = 0; i < m_programsOffered.size(); ++i) {
        ss << "\"" << m_programsOffered[i] << "\"" << (i + 1 < m_programsOffered.size() ? "," : "");
    }
    ss << "]}";
    return ss.str();
}

void DepartmentEntity::printSummary(std::ostream& os) const {
    os << "[Department] " << getCode() << " - " << getName() 
       << " (HOD: " << m_hodName << ", Programs: " << m_programsOffered.size() << ")";
}
