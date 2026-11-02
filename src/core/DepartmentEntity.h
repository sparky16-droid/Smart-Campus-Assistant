#ifndef DEPARTMENT_ENTITY_H
#define DEPARTMENT_ENTITY_H

#include "CampusEntity.h"
#include <vector>

/**
 * @file DepartmentEntity.h
 * @brief Represents an academic department.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Specializes CampusEntity with academic degree programs and administration attributes.
 * - POLYMORPHISM: Overrides getDetails(), calculateRelevance(), serializeJson(), and printSummary().
 * - ENCAPSULATION: Protects program rosters, student enrolment stats, and HOD contacts.
 */
class DepartmentEntity : public CampusEntity {
private:
    std::string m_buildingId;
    std::string m_buildingName;
    std::string m_hodName;
    std::string m_contactEmail;
    std::string m_contactPhone;
    std::string m_websiteUrl;
    std::vector<std::string> m_programsOffered;
    int m_studentCount;
    int m_facultyCount;

public:
    DepartmentEntity(std::string id, std::string name, std::string code, std::string description,
                     std::string buildingId, std::string buildingName, std::string hodName,
                     std::string contactEmail, std::string contactPhone, std::string websiteUrl,
                     std::vector<std::string> programs, int studentCount, int facultyCount);

    virtual ~DepartmentEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "Department"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getBuildingId() const { return m_buildingId; }
    std::string getBuildingName() const { return m_buildingName; }
    std::string getHodName() const { return m_hodName; }
    std::string getContactEmail() const { return m_contactEmail; }
    std::string getContactPhone() const { return m_contactPhone; }
    std::string getWebsiteUrl() const { return m_websiteUrl; }
    const std::vector<std::string>& getPrograms() const { return m_programsOffered; }
    int getStudentCount() const { return m_studentCount; }
    int getFacultyCount() const { return m_facultyCount; }

    // Mutators
    void setHodName(const std::string& hod);
    void addProgram(const std::string& program);
    void setCounts(int students, int faculty);
};

#endif // DEPARTMENT_ENTITY_H
