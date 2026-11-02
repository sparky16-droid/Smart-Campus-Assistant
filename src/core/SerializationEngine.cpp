#include "SerializationEngine.h"
#include <fstream>
#include <sstream>

std::string SerializationEngine::exportBuildingsToJson(const std::vector<std::shared_ptr<BuildingEntity>>& buildings) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < buildings.size(); ++i) {
        ss << "  " << buildings[i]->serializeJson() << (i + 1 < buildings.size() ? ",\n" : "\n");
    }
    ss << "]\n";
    return ss.str();
}

std::string SerializationEngine::exportFacilitiesToJson(const std::vector<std::shared_ptr<FacilityEntity>>& facilities) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < facilities.size(); ++i) {
        ss << "  " << facilities[i]->serializeJson() << (i + 1 < facilities.size() ? ",\n" : "\n");
    }
    ss << "]\n";
    return ss.str();
}

std::string SerializationEngine::exportNoticesToJson(const std::vector<std::shared_ptr<NoticeEntity>>& notices) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < notices.size(); ++i) {
        ss << "  " << notices[i]->serializeJson() << (i + 1 < notices.size() ? ",\n" : "\n");
    }
    ss << "]\n";
    return ss.str();
}

std::string SerializationEngine::exportFaqsToJson(const std::vector<std::shared_ptr<FaqEntity>>& faqs) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < faqs.size(); ++i) {
        ss << "  " << faqs[i]->serializeJson() << (i + 1 < faqs.size() ? ",\n" : "\n");
    }
    ss << "]\n";
    return ss.str();
}

std::string SerializationEngine::exportBuildingsToCsv(const std::vector<std::shared_ptr<BuildingEntity>>& buildings) {
    std::stringstream ss;
    ss << "id,code,name,floors,yearBuilt,wheelchairAccessible,latitude,longitude\n";
    for (const auto& b : buildings) {
        ss << "\"" << b->getId() << "\","
           << "\"" << b->getCode() << "\","
           << "\"" << b->getName() << "\","
           << b->getFloors() << ","
           << b->getYearBuilt() << ","
           << (b->isWheelchairAccessible() ? "true" : "false") << ","
           << b->getLatitude() << ","
           << b->getLongitude() << "\n";
    }
    return ss.str();
}

std::string SerializationEngine::exportFacilitiesToCsv(const std::vector<std::shared_ptr<FacilityEntity>>& facilities) {
    std::stringstream ss;
    ss << "id,name,category,buildingName,floor,roomNumber,timings,isOperational\n";
    for (const auto& f : facilities) {
        ss << "\"" << f->getId() << "\","
           << "\"" << f->getName() << "\","
           << "\"" << f->getCategory() << "\","
           << "\"" << f->getBuildingName() << "\","
           << "\"" << f->getFloor() << "\","
           << "\"" << f->getRoomNumber() << "\","
           << "\"" << f->getTimings() << "\","
           << (f->isOperational() ? "true" : "false") << "\n";
    }
    return ss.str();
}

std::string SerializationEngine::exportFacultyToCsv(const std::vector<std::shared_ptr<FacultyEntity>>& faculty) {
    std::stringstream ss;
    ss << "id,name,title,designation,departmentName,cabinRoom,email,phone\n";
    for (const auto& fac : faculty) {
        ss << "\"" << fac->getId() << "\","
           << "\"" << fac->getName() << "\","
           << "\"" << fac->getTitle() << "\","
           << "\"" << fac->getDesignation() << "\","
           << "\"" << fac->getDepartmentName() << "\","
           << "\"" << fac->getCabinRoom() << "\","
           << "\"" << fac->getEmail() << "\","
           << "\"" << fac->getPhone() << "\"\n";
    }
    return ss.str();
}

bool SerializationEngine::writeStringToFile(const std::string& filePath, const std::string& data) {
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        throw SerializationException("Could not open file for writing: " + filePath);
    }
    outFile << data;
    outFile.close();
    return true;
}

std::string SerializationEngine::readStringFromFile(const std::string& filePath) {
    std::ifstream inFile(filePath, std::ios::in);
    if (!inFile.is_open()) {
        throw SerializationException("Could not open file for reading: " + filePath);
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();
    return buffer.str();
}
