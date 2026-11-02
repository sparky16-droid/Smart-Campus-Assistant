#ifndef FACILITY_ENTITY_H
#define FACILITY_ENTITY_H

#include "CampusEntity.h"

/**
 * @file FacilityEntity.h
 * @brief Represents a campus amenity or service facility.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Inherits core entity characteristics from CampusEntity.
 * - POLYMORPHISM: Custom implementation of getDetails(), relevance calculation, and JSON serialization.
 * - ENCAPSULATION: Strict tracking of operational status, operating hours, and location pointers.
 */
class FacilityEntity : public CampusEntity {
private:
    std::string m_category; // Academic, Library, Cafeteria, Sports, Lab, Hostel, Medical, ATM, Admin
    std::string m_buildingId;
    std::string m_buildingName;
    std::string m_floor;
    std::string m_roomNumber;
    std::string m_landmark;
    std::string m_timings;
    std::string m_contactPhone;
    std::string m_contactEmail;
    bool m_isOperational;

public:
    FacilityEntity(std::string id, std::string name, std::string category, std::string description,
                   std::string buildingId, std::string buildingName, std::string floor,
                   std::string roomNumber, std::string landmark, std::string timings,
                   std::string contactPhone, std::string contactEmail, bool isOperational);

    virtual ~FacilityEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "Facility"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getCategory() const { return m_category; }
    std::string getBuildingId() const { return m_buildingId; }
    std::string getBuildingName() const { return m_buildingName; }
    std::string getFloor() const { return m_floor; }
    std::string getRoomNumber() const { return m_roomNumber; }
    std::string getLandmark() const { return m_landmark; }
    std::string getTimings() const { return m_timings; }
    std::string getContactPhone() const { return m_contactPhone; }
    std::string getContactEmail() const { return m_contactEmail; }
    bool isOperational() const { return m_isOperational; }

    // Mutators
    void setOperational(bool operational);
    void setTimings(const std::string& timings);
    void setLandmark(const std::string& landmark);
};

#endif // FACILITY_ENTITY_H
