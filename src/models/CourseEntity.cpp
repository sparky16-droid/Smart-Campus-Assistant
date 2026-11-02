#include "CourseEntity.h"
#include <algorithm>
#include <cctype>

CourseEntity::CourseEntity(std::string id, std::string courseTitle, std::string courseCode, std::string description,
                           std::string departmentId, std::string departmentName, int credits, int semester,
                           std::vector<std::string> prerequisites, std::vector<std::string> syllabus, bool isElective)
    : CampusEntity(std::move(id), std::move(courseTitle), std::move(courseCode), std::move(description)),
      m_departmentId(std::move(departmentId)),
      m_departmentName(std::move(departmentName)),
      m_credits(credits),
      m_semester(semester),
      m_prerequisites(std::move(prerequisites)),
      m_syllabusModules(std::move(syllabus)),
      m_isElective(isElective)
{
}

void CourseEntity::addPrerequisite(const std::string& courseCode) {
    if (!courseCode.empty()) {
        m_prerequisites.push_back(courseCode);
        touch();
    }
}

void CourseEntity::addSyllabusModule(const std::string& moduleTitle) {
    if (!moduleTitle.empty()) {
        m_syllabusModules.push_back(moduleTitle);
        touch();
    }
}

std::string CourseEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== COURSE: " << getName() << " (" << getCode() << ") ===\n"
       << "Department: " << m_departmentName << " | Semester: " << m_semester 
       << " | Credits: " << m_credits << " | Type: " << (m_isElective ? "Elective" : "Core Mandatory") << "\n"
       << "Description: " << getDescription() << "\n";

    if (!m_prerequisites.empty()) {
        ss << "Prerequisites: ";
        for (size_t i = 0; i < m_prerequisites.size(); ++i) {
            ss << m_prerequisites[i] << (i + 1 < m_prerequisites.size() ? ", " : "\n");
        }
    }

    if (!m_syllabusModules.empty()) {
        ss << "Curriculum Syllabus Modules:\n";
        for (size_t i = 0; i < m_syllabusModules.size(); ++i) {
            ss << "  Unit " << (i + 1) << ": " << m_syllabusModules[i] << "\n";
        }
    }
    return ss.str();
}

static std::string toLowerCourse(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double CourseEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerCourse(query);
    std::string n = toLowerCourse(getName());
    std::string c = toLowerCourse(getCode());
    std::string d = toLowerCourse(m_departmentName);

    if (n == q || c == q) return 1.0;
    if (n.find(q) != std::string::npos) return 0.85;
    if (c.find(q) != std::string::npos) return 0.90;
    if (d.find(q) != std::string::npos) return 0.60;

    for (const auto& mod : m_syllabusModules) {
        if (toLowerCourse(mod).find(q) != std::string::npos) return 0.75;
    }
    return 0.0;
}

std::string CourseEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"title\":\"" << getName() << "\","
       << "\"code\":\"" << getCode() << "\","
       << "\"description\":\"" << getDescription() << "\","
       << "\"departmentId\":\"" << m_departmentId << "\","
       << "\"departmentName\":\"" << m_departmentName << "\","
       << "\"credits\":" << m_credits << ","
       << "\"semester\":" << m_semester << ","
       << "\"isElective\":" << (m_isElective ? "true" : "false")
       << "}";
    return ss.str();
}

void CourseEntity::printSummary(std::ostream& os) const {
    os << "[Course] " << getCode() << ": " << getName() << " (" << m_credits << " Credits, Sem " << m_semester << ")";
}
