#include "FacultyEntity.h"
#include <algorithm>
#include <cctype>

FacultyEntity::FacultyEntity(std::string id, std::string name, std::string title, std::string designation,
                             std::string departmentId, std::string departmentName, std::string email,
                             std::string phone, std::string cabinRoom, std::string officeHours,
                             std::vector<std::string> specializations, std::string biography,
                             int publicationsCount)
    : CampusEntity(std::move(id), std::move(name), "", ""),
      m_title(std::move(title)),
      m_designation(std::move(designation)),
      m_departmentId(std::move(departmentId)),
      m_departmentName(std::move(departmentName)),
      m_email(std::move(email)),
      m_phone(std::move(phone)),
      m_cabinRoom(std::move(cabinRoom)),
      m_officeHours(std::move(officeHours)),
      m_specializations(std::move(specializations)),
      m_biography(std::move(biography)),
      m_publicationsCount(publicationsCount)
{
}

void FacultyEntity::setOfficeHours(const std::string& hours) {
    m_officeHours = hours;
    touch();
}

void FacultyEntity::setCabinRoom(const std::string& cabin) {
    m_cabinRoom = cabin;
    touch();
}

void FacultyEntity::addSpecialization(const std::string& spec) {
    if (!spec.empty()) {
        m_specializations.push_back(spec);
        touch();
    }
}

std::string FacultyEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== FACULTY: " << m_title << " " << getName() << " ===\n"
       << "Designation: " << m_designation << "\n"
       << "Department: " << m_departmentName << "\n"
       << "Cabin / Office: " << m_cabinRoom << "\n"
       << "Office Hours: " << m_officeHours << "\n"
       << "Email: " << m_email << " | Phone: " << m_phone << "\n"
       << "Publications: " << m_publicationsCount << " papers indexed\n"
       << "Research Specializations: ";
    for (size_t i = 0; i < m_specializations.size(); ++i) {
        ss << m_specializations[i] << (i + 1 < m_specializations.size() ? ", " : "\n");
    }
    ss << "Bio: " << m_biography << "\n";
    return ss.str();
}

static std::string toLowerFaculty(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double FacultyEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerFaculty(query);
    std::string n = toLowerFaculty(getName());
    std::string d = toLowerFaculty(m_departmentName);
    std::string des = toLowerFaculty(m_designation);

    if (n == q) return 1.0;
    if (n.find(q) != std::string::npos) return 0.90;
    if (d.find(q) != std::string::npos) return 0.70;
    if (des.find(q) != std::string::npos) return 0.60;

    for (const auto& spec : m_specializations) {
        if (toLowerFaculty(spec).find(q) != std::string::npos) return 0.80;
    }
    return 0.0;
}

std::string FacultyEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"name\":\"" << getName() << "\","
       << "\"title\":\"" << m_title << "\","
       << "\"designation\":\"" << m_designation << "\","
       << "\"departmentId\":\"" << m_departmentId << "\","
       << "\"departmentName\":\"" << m_departmentName << "\","
       << "\"email\":\"" << m_email << "\","
       << "\"phone\":\"" << m_phone << "\","
       << "\"cabinRoom\":\"" << m_cabinRoom << "\","
       << "\"officeHours\":\"" << m_officeHours << "\","
       << "\"publicationsCount\":" << m_publicationsCount << ","
       << "\"specializations\":[";
    for (size_t i = 0; i < m_specializations.size(); ++i) {
        ss << "\"" << m_specializations[i] << "\"" << (i + 1 < m_specializations.size() ? "," : "");
    }
    ss << "],\"biography\":\"" << m_biography << "\"}";
    return ss.str();
}

void FacultyEntity::printSummary(std::ostream& os) const {
    os << "[Faculty] " << m_title << " " << getName() << " (" << m_designation 
       << ", " << m_departmentName << ") Cabin: " << m_cabinRoom;
}
