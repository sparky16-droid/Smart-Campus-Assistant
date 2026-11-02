#ifndef FACULTY_ENTITY_H
#define FACULTY_ENTITY_H

#include "CampusEntity.h"
#include <vector>

/**
 * @file FacultyEntity.h
 * @brief Represents a university professor or lecturer.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Inherits core entity identity from CampusEntity.
 * - POLYMORPHISM: Overrides getDetails(), calculateRelevance(), serializeJson(), and printSummary().
 * - ENCAPSULATION: Protects office hours, cabin room locations, and contact info.
 */
class FacultyEntity : public CampusEntity {
private:
    std::string m_title; // Dr., Prof., Assoc. Prof.
    std::string m_designation;
    std::string m_departmentId;
    std::string m_departmentName;
    std::string m_email;
    std::string m_phone;
    std::string m_cabinRoom;
    std::string m_officeHours;
    std::vector<std::string> m_specializations;
    std::string m_biography;
    int m_publicationsCount;

public:
    FacultyEntity(std::string id, std::string name, std::string title, std::string designation,
                  std::string departmentId, std::string departmentName, std::string email,
                  std::string phone, std::string cabinRoom, std::string officeHours,
                  std::vector<std::string> specializations, std::string biography,
                  int publicationsCount);

    virtual ~FacultyEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "Faculty"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getTitle() const { return m_title; }
    std::string getDesignation() const { return m_designation; }
    std::string getDepartmentId() const { return m_departmentId; }
    std::string getDepartmentName() const { return m_departmentName; }
    std::string getEmail() const { return m_email; }
    std::string getPhone() const { return m_phone; }
    std::string getCabinRoom() const { return m_cabinRoom; }
    std::string getOfficeHours() const { return m_officeHours; }
    const std::vector<std::string>& getSpecializations() const { return m_specializations; }
    std::string getBiography() const { return m_biography; }
    int getPublicationsCount() const { return m_publicationsCount; }

    // Mutators
    void setOfficeHours(const std::string& hours);
    void setCabinRoom(const std::string& cabin);
    void addSpecialization(const std::string& spec);
};

#endif // FACULTY_ENTITY_H
