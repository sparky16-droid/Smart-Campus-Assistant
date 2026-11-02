#ifndef COURSE_ENTITY_H
#define COURSE_ENTITY_H

#include "../core/CampusEntity.h"
#include <vector>

/**
 * @file CourseEntity.h
 * @brief Represents an academic curriculum course / syllabus subject.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Inherits from CampusEntity.
 * - POLYMORPHISM: Overrides getDetails(), calculateRelevance(), serializeJson(), and printSummary().
 * - ENCAPSULATION: Protects prerequisite trees, credit points, and syllabus units.
 */
class CourseEntity : public CampusEntity {
private:
    std::string m_departmentId;
    std::string m_departmentName;
    int m_credits;
    int m_semester;
    std::vector<std::string> m_prerequisites;
    std::vector<std::string> m_syllabusModules;
    bool m_isElective;

public:
    CourseEntity(std::string id, std::string courseTitle, std::string courseCode, std::string description,
                 std::string departmentId, std::string departmentName, int credits, int semester,
                 std::vector<std::string> prerequisites, std::vector<std::string> syllabus, bool isElective);

    virtual ~CourseEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "Course"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getDepartmentId() const { return m_departmentId; }
    std::string getDepartmentName() const { return m_departmentName; }
    int getCredits() const { return m_credits; }
    int getSemester() const { return m_semester; }
    const std::vector<std::string>& getPrerequisites() const { return m_prerequisites; }
    const std::vector<std::string>& getSyllabus() const { return m_syllabusModules; }
    bool isElective() const { return m_isElective; }

    // Mutators
    void addPrerequisite(const std::string& courseCode);
    void addSyllabusModule(const std::string& moduleTitle);
};

#endif // COURSE_ENTITY_H
