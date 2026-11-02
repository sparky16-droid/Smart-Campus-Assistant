#include "EntityFactory.h"

std::shared_ptr<BuildingEntity> EntityFactory::createBuilding(
    std::string id, std::string name, std::string code, std::string description,
    double latitude, double longitude, int floors, int yearBuilt,
    bool wheelchairAccessible, int mapX, int mapY, int mapWidth, int mapHeight,
    std::string colorHex)
{
    return std::make_shared<BuildingEntity>(
        std::move(id), std::move(name), std::move(code), std::move(description),
        latitude, longitude, floors, yearBuilt, wheelchairAccessible,
        mapX, mapY, mapWidth, mapHeight, std::move(colorHex)
    );
}

std::shared_ptr<FacilityEntity> EntityFactory::createFacility(
    std::string id, std::string name, std::string category, std::string description,
    std::string buildingId, std::string buildingName, std::string floor,
    std::string roomNumber, std::string landmark, std::string timings,
    std::string contactPhone, std::string contactEmail, bool isOperational)
{
    return std::make_shared<FacilityEntity>(
        std::move(id), std::move(name), std::move(category), std::move(description),
        std::move(buildingId), std::move(buildingName), std::move(floor),
        std::move(roomNumber), std::move(landmark), std::move(timings),
        std::move(contactPhone), std::move(contactEmail), isOperational
    );
}

std::shared_ptr<DepartmentEntity> EntityFactory::createDepartment(
    std::string id, std::string name, std::string code, std::string description,
    std::string buildingId, std::string buildingName, std::string hodName,
    std::string contactEmail, std::string contactPhone, std::string websiteUrl,
    std::vector<std::string> programs, int studentCount, int facultyCount)
{
    return std::make_shared<DepartmentEntity>(
        std::move(id), std::move(name), std::move(code), std::move(description),
        std::move(buildingId), std::move(buildingName), std::move(hodName),
        std::move(contactEmail), std::move(contactPhone), std::move(websiteUrl),
        std::move(programs), studentCount, facultyCount
    );
}

std::shared_ptr<FacultyEntity> EntityFactory::createFaculty(
    std::string id, std::string name, std::string title, std::string designation,
    std::string departmentId, std::string departmentName, std::string email,
    std::string phone, std::string cabinRoom, std::string officeHours,
    std::vector<std::string> specializations, std::string biography,
    int publicationsCount)
{
    return std::make_shared<FacultyEntity>(
        std::move(id), std::move(name), std::move(title), std::move(designation),
        std::move(departmentId), std::move(departmentName), std::move(email),
        std::move(phone), std::move(cabinRoom), std::move(officeHours),
        std::move(specializations), std::move(biography), publicationsCount
    );
}

std::shared_ptr<NoticeEntity> EntityFactory::createNotice(
    std::string id, std::string title, std::string content, std::string category,
    NoticePriority priority, bool isPublished, std::string authorName,
    uint64_t publishDateMs)
{
    return std::make_shared<NoticeEntity>(
        std::move(id), std::move(title), std::move(content), std::move(category),
        priority, isPublished, std::move(authorName), publishDateMs
    );
}

std::shared_ptr<FaqEntity> EntityFactory::createFaq(
    std::string id, std::string question, std::string answer, std::string category,
    std::vector<std::string> keywords, int viewCount, int helpfulCount)
{
    return std::make_shared<FaqEntity>(
        std::move(id), std::move(question), std::move(answer), std::move(category),
        std::move(keywords), viewCount, helpfulCount
    );
}
