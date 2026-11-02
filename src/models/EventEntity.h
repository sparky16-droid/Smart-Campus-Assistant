#ifndef EVENT_ENTITY_H
#define EVENT_ENTITY_H

#include "../core/CampusEntity.h"

/**
 * @file EventEntity.h
 * @brief Represents campus events, workshops, hackathons, and guest lectures.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - INHERITANCE: Inherits from CampusEntity.
 * - POLYMORPHISM: Overrides getDetails(), calculateRelevance(), serializeJson(), and printSummary().
 * - ENCAPSULATION: Encapsulates ticket registrations, venue location pointers, and schedule.
 */
class EventEntity : public CampusEntity {
private:
    std::string m_venueBuildingId;
    std::string m_venueBuildingName;
    std::string m_venueHall;
    std::string m_dateTimeStr;
    std::string m_organizingClub;
    std::string m_keynoteSpeaker;
    int m_maxCapacity;
    int m_registeredCount;
    double m_registrationFee;

public:
    EventEntity(std::string id, std::string title, std::string description,
                std::string venueId, std::string venueName, std::string hall,
                std::string dateTime, std::string club, std::string speaker,
                int maxCapacity, double fee = 0.0);

    virtual ~EventEntity() override = default;

    // Polymorphic overrides
    std::string getEntityType() const override { return "Event"; }
    std::string getDetails() const override;
    double calculateRelevance(const std::string& query) const override;
    std::string serializeJson() const override;
    void printSummary(std::ostream& os) const override;

    // Getters
    std::string getVenueBuildingName() const { return m_venueBuildingName; }
    std::string getVenueHall() const { return m_venueHall; }
    std::string getDateTime() const { return m_dateTimeStr; }
    std::string getOrganizingClub() const { return m_organizingClub; }
    std::string getKeynoteSpeaker() const { return m_keynoteSpeaker; }
    int getMaxCapacity() const { return m_maxCapacity; }
    int getRegisteredCount() const { return m_registeredCount; }
    double getRegistrationFee() const { return m_registrationFee; }

    // Mutators
    bool registerAttendee();
    bool isSoldOut() const { return m_registeredCount >= m_maxCapacity; }
};

#endif // EVENT_ENTITY_H
