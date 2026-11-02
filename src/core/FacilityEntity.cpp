#include "FacilityEntity.h"
#include <algorithm>
#include <cctype>

FacilityEntity::FacilityEntity(std::string id, std::string name, std::string category, std::string description,
                               std::string buildingId, std::string buildingName, std::string floor,
                               std::string roomNumber, std::string landmark, std::string timings,
                               std::string contactPhone, std::string contactEmail, bool isOperational)
    : CampusEntity(std::move(id), std::move(name), "", std::move(description)),
      m_category(std::move(category)),
      m_buildingId(std::move(buildingId)),
      m_buildingName(std::move(buildingName)),
      m_floor(std::move(floor)),
      m_roomNumber(std::move(roomNumber)),
      m_landmark(std::move(landmark)),
      m_timings(std::move(timings)),
      m_contactPhone(std::move(contactPhone)),
      m_contactEmail(std::move(contactEmail)),
      m_isOperational(isOperational)
{
}

void FacilityEntity::setOperational(bool operational) {
    m_isOperational = operational;
    touch();
}

void FacilityEntity::setTimings(const std::string& timings) {
    m_timings = timings;
    touch();
}

void FacilityEntity::setLandmark(const std::string& landmark) {
    m_landmark = landmark;
    touch();
}

std::string FacilityEntity::getDetails() const {
    std::stringstream ss;
    ss << "=== FACILITY: " << getName() << " [" << m_category << "] ===\n"
       << "Building: " << m_buildingName << " | Floor: " << m_floor << " | Room: " << m_roomNumber << "\n"
       << "Landmark: " << m_landmark << "\n"
       << "Operating Timings: " << m_timings << "\n"
       << "Status: " << (m_isOperational ? "OPEN / OPERATIONAL" : "CLOSED / MAINTENANCE") << "\n"
       << "Contact: " << m_contactPhone << " | " << m_contactEmail << "\n"
       << "Description: " << getDescription() << "\n";
    return ss.str();
}

static std::string toLowerFacility(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

double FacilityEntity::calculateRelevance(const std::string& query) const {
    if (query.empty()) return 0.0;
    std::string q = toLowerFacility(query);
    std::string n = toLowerFacility(getName());
    std::string c = toLowerFacility(m_category);
    std::string b = toLowerFacility(m_buildingName);
    std::string l = toLowerFacility(m_landmark);
    std::string d = toLowerFacility(getDescription());

    if (n == q) return 1.0;
    if (n.find(q) != std::string::npos) return 0.85;
    if (c == q || c.find(q) != std::string::npos) return 0.75;
    if (b.find(q) != std::string::npos) return 0.60;
    if (l.find(q) != std::string::npos) return 0.50;
    if (d.find(q) != std::string::npos) return 0.40;
    return 0.0;
}

std::string FacilityEntity::serializeJson() const {
    std::stringstream ss;
    ss << "{"
       << "\"id\":\"" << getId() << "\","
       << "\"name\":\"" << getName() << "\","
       << "\"category\":\"" << m_category << "\","
       << "\"description\":\"" << getDescription() << "\","
       << "\"buildingId\":\"" << m_buildingId << "\","
       << "\"buildingName\":\"" << m_buildingName << "\","
       << "\"floor\":\"" << m_floor << "\","
       << "\"roomNumber\":\"" << m_roomNumber << "\","
       << "\"landmark\":\"" << m_landmark << "\","
       << "\"timings\":\"" << m_timings << "\","
       << "\"contactPhone\":\"" << m_contactPhone << "\","
       << "\"contactEmail\":\"" << m_contactEmail << "\","
       << "\"isOperational\":" << (m_isOperational ? "true" : "false")
       << "}";
    return ss.str();
}

void FacilityEntity::printSummary(std::ostream& os) const {
    os << "[Facility - " << m_category << "] " << getName() 
       << " @ " << m_buildingName << " (" << m_floor << ") [" 
       << (m_isOperational ? "Active" : "Closed") << "]";
}
