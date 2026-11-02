#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "ICampusEntity.h"
#include "BuildingEntity.h"
#include "FacilityEntity.h"
#include "DepartmentEntity.h"
#include "FacultyEntity.h"
#include "NoticeEntity.h"
#include "FaqEntity.h"
#include <memory>
#include <string>

/**
 * @file EntityFactory.h
 * @brief Factory class for polymorphic instantiation of campus entities.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: FACTORY DESIGN PATTERN & POLYMORPHISM
 * - Encapsulates the instantiation logic of derived entity classes.
 * - Callers request entities by type code without knowing exact constructor signatures.
 */
class EntityFactory {
public:
    static std::shared_ptr<BuildingEntity> createBuilding(
        std::string id, std::string name, std::string code, std::string description,
        double latitude, double longitude, int floors, int yearBuilt,
        bool wheelchairAccessible, int mapX, int mapY, int mapWidth, int mapHeight,
        std::string colorHex);

    static std::shared_ptr<FacilityEntity> createFacility(
        std::string id, std::string name, std::string category, std::string description,
        std::string buildingId, std::string buildingName, std::string floor,
        std::string roomNumber, std::string landmark, std::string timings,
        std::string contactPhone, std::string contactEmail, bool isOperational);

    static std::shared_ptr<DepartmentEntity> createDepartment(
        std::string id, std::string name, std::string code, std::string description,
        std::string buildingId, std::string buildingName, std::string hodName,
        std::string contactEmail, std::string contactPhone, std::string websiteUrl,
        std::vector<std::string> programs, int studentCount, int facultyCount);

    static std::shared_ptr<FacultyEntity> createFaculty(
        std::string id, std::string name, std::string title, std::string designation,
        std::string departmentId, std::string departmentName, std::string email,
        std::string phone, std::string cabinRoom, std::string officeHours,
        std::vector<std::string> specializations, std::string biography,
        int publicationsCount);

    static std::shared_ptr<NoticeEntity> createNotice(
        std::string id, std::string title, std::string content, std::string category,
        NoticePriority priority, bool isPublished, std::string authorName,
        uint64_t publishDateMs = 0);

    static std::shared_ptr<FaqEntity> createFaq(
        std::string id, std::string question, std::string answer, std::string category,
        std::vector<std::string> keywords, int viewCount = 0, int helpfulCount = 0);
};

#endif // ENTITY_FACTORY_H
