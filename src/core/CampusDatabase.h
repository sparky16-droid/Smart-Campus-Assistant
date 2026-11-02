#ifndef CAMPUS_DATABASE_H
#define CAMPUS_DATABASE_H

#include "CampusRepository.h"
#include "BuildingEntity.h"
#include "FacilityEntity.h"
#include "DepartmentEntity.h"
#include "FacultyEntity.h"
#include "NoticeEntity.h"
#include "FaqEntity.h"
#include "Observer.h"
#include <memory>
#include <string>

/**
 * @file CampusDatabase.h
 * @brief Central In-Memory Database and Knowledge Core.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS:
 * - SINGLETON DESIGN PATTERN: Guarantees a single source of truth across the campus assistant.
 * - TEMPLATE REPOSITORIES: Uses CampusRepository<T> for type-safe polymorphic storage.
 * - OBSERVER INTEGRATION: Publishes new notices via CampusNoticePublisher.
 */
class CampusDatabase {
private:
    CampusRepository<BuildingEntity> m_buildings;
    CampusRepository<FacilityEntity> m_facilities;
    CampusRepository<DepartmentEntity> m_departments;
    CampusRepository<FacultyEntity> m_faculty;
    CampusRepository<NoticeEntity> m_notices;
    CampusRepository<FaqEntity> m_faqs;
    CampusNoticePublisher m_noticePublisher;

    bool m_initialized;
    std::string m_adminPasswordHash;

    // Private constructor for Singleton
    CampusDatabase();

    void seedDefaultData();

public:
    // Delete copy and move constructors
    CampusDatabase(const CampusDatabase&) = delete;
    CampusDatabase& operator=(const CampusDatabase&) = delete;
    CampusDatabase(CampusDatabase&&) = delete;
    CampusDatabase& operator=(CampusDatabase&&) = delete;

    // Global access point
    static CampusDatabase& instance();

    // Initialization & seeding
    void initialize();
    bool isInitialized() const { return m_initialized; }

    // Repository accessors
    CampusRepository<BuildingEntity>& buildings() { return m_buildings; }
    CampusRepository<FacilityEntity>& facilities() { return m_facilities; }
    CampusRepository<DepartmentEntity>& departments() { return m_departments; }
    CampusRepository<FacultyEntity>& faculty() { return m_faculty; }
    CampusRepository<NoticeEntity>& notices() { return m_notices; }
    CampusRepository<FaqEntity>& faqs() { return m_faqs; }
    CampusNoticePublisher& noticePublisher() { return m_noticePublisher; }

    // Admin authentication
    bool verifyAdmin(const std::string& username, const std::string& password) const;

    // Cross-entity polymorphic search
    std::vector<std::shared_ptr<ICampusEntity>> searchAll(const std::string& query, double minRelevance = 0.40) const;

    // Quick stats summary
    std::string getSystemStats() const;
};

#endif // CAMPUS_DATABASE_H
