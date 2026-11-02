#include "EventEntity.h"
#include <algorithm>
#include <cctype>

EventEntity::EventEntity(std::string id, std::string title, std::string description,
                         std::string venueId, std::string venueName, std::string hall,
                         std::string dateTime, std::string club, std::string speaker,
                         int maxCapacity, double fee)
    : CampusEntity(std::move(id), std::move(title), "", std::move(description)),
      m_venueBuildingId(std::move(venueId)),
      m_venueBuildingName(std::move(venueName)),
      m_venueHall(std::move(hall)),
      m_dateTimeStr(std::move(dateTime)),
      m_organizingClub(std::move(club)),
      m_keynoteSpeaker(std::move(speaker)),
      m_maxCapacity(maxCapacity),
      m_registeredCount(0),
      m_registrationFee(fee)
{
}

bool EventEntity::registerAttendee() {
    if (m_registeredCount < m_maxCapacity) {
        m_registeredCount++;
        touch();
        return true;
    }
    return false;
}

std::string EventEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== CAMPUS EVENT: " << getName() << " ===\n"
       << "Venue: " << m_venueBuildingName << " (" << m_venueHall << ")\n"
       << "Date & Time: " << m_dateTimeStr << "\n"
       << "Organized By: " << m_organizingClub << "\n"
       << "Keynote Speaker: " << m_keynoteSpeaker << "\n"
       << "Capacity: " << m_registeredCount << " / " << m_maxCapacity << " registered\n"
       << "Registration Fee: " << (m_registrationFee > 0.0 ? ("Rs " + std::to_string(static_cast<int>(m_registrationFee))) : "FREE") << "\n"
       << "Description: " << getDescription() << "\n";
    return ss.str();
}

static std::string toLowerEvent(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double EventEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerEvent(query);
    std::string n = toLowerEvent(getName());
    std::string c = toLowerEvent(m_organizingClub);
    std::string s = toLowerEvent(m_keynoteSpeaker);
    std::string v = toLowerEvent(m_venueBuildingName);

    if (n == q) return 1.0;
    if (n.find(q) != std::string::npos) return 0.85;
    if (c.find(q) != std::string::npos) return 0.70;
    if (s.find(q) != std::string::npos) return 0.80;
    if (v.find(q) != std::string::npos) return 0.65;
    return 0.0;
}

std::string EventEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"title\":\"" << getName() << "\","
       << "\"description\":\"" << getDescription() << "\","
       << "\"venueBuildingName\":\"" << m_venueBuildingName << "\","
       << "\"venueHall\":\"" << m_venueHall << "\","
       << "\"dateTime\":\"" << m_dateTimeStr << "\","
       << "\"organizingClub\":\"" << m_organizingClub << "\","
       << "\"keynoteSpeaker\":\"" << m_keynoteSpeaker << "\","
       << "\"maxCapacity\":" << m_maxCapacity << ","
       << "\"registeredCount\":" << m_registeredCount << ","
       << "\"registrationFee\":" << m_registrationFee
       << "}";
    return ss.str();
}

void EventEntity::printSummary(std::ostream& os) const {
    os << "[Event] " << getName() << " @ " << m_venueHall 
       << " (" << m_dateTimeStr << ") [" << m_registeredCount << "/" << m_maxCapacity << "]";
}
