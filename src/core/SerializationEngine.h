#ifndef SERIALIZATION_ENGINE_H
#define SERIALIZATION_ENGINE_H

#include "BuildingEntity.h"
#include "FacilityEntity.h"
#include "DepartmentEntity.h"
#include "FacultyEntity.h"
#include "NoticeEntity.h"
#include "FaqEntity.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

class SerializationException : public std::runtime_error {
public:
    explicit SerializationException(const std::string& message)
        : std::runtime_error("SerializationException: " + message) {}
};

/**
 * @file SerializationEngine.h
 * @brief File stream serialization and deserialization engine in pure standard C++.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS: FILE STREAMS, EXCEPTION HANDLING & PARSING
 * - Exports and imports database records to/from JSON and CSV files.
 * - Handles file stream errors via custom SerializationException hierarchy.
 */
class SerializationEngine {
public:
    // JSON Serialization
    static std::string exportBuildingsToJson(const std::vector<std::shared_ptr<BuildingEntity>>& buildings);
    static std::string exportFacilitiesToJson(const std::vector<std::shared_ptr<FacilityEntity>>& facilities);
    static std::string exportNoticesToJson(const std::vector<std::shared_ptr<NoticeEntity>>& notices);
    static std::string exportFaqsToJson(const std::vector<std::shared_ptr<FaqEntity>>& faqs);

    // CSV Serialization
    static std::string exportBuildingsToCsv(const std::vector<std::shared_ptr<BuildingEntity>>& buildings);
    static std::string exportFacilitiesToCsv(const std::vector<std::shared_ptr<FacilityEntity>>& facilities);
    static std::string exportFacultyToCsv(const std::vector<std::shared_ptr<FacultyEntity>>& faculty);

    // File Stream Writing
    static bool writeStringToFile(const std::string& filePath, const std::string& data);
    static std::string readStringFromFile(const std::string& filePath);
};

#endif // SERIALIZATION_ENGINE_H
