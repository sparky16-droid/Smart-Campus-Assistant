#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QMap>
#include "Models.h"

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    static DatabaseManager& instance();

    bool initialize(const QString& dbPath = "smart_campus.db");
    void seedDatabaseIfEmpty();

    // Buildings
    QList<Building> getAllBuildings();
    Building getBuildingById(const QString& id);
    bool addBuilding(const Building& b);
    bool updateBuilding(const Building& b);
    bool deleteBuilding(const QString& id);

    // Facilities
    QList<Facility> getAllFacilities(const QString& categoryFilter = "");
    Facility getFacilityById(const QString& id);
    bool addFacility(const Facility& f);
    bool updateFacility(const Facility& f);
    bool deleteFacility(const QString& id);

    // Departments
    QList<Department> getAllDepartments();
    Department getDepartmentByCode(const QString& code);
    bool addDepartment(const Department& d);
    bool updateDepartment(const Department& d);
    bool deleteDepartment(const QString& id);

    // Faculty
    QList<Faculty> getAllFaculty(const QString& departmentIdFilter = "");
    Faculty getFacultyById(const QString& id);
    bool addFaculty(const Faculty& f);
    bool updateFaculty(const Faculty& f);
    bool deleteFaculty(const QString& id);

    // Notices
    QList<Notice> getAllNotices(const QString& categoryFilter = "");
    bool addNotice(const Notice& n);
    bool updateNotice(const Notice& n);
    bool deleteNotice(const QString& id);

    // FAQs
    QList<Faq> getAllFaqs(const QString& categoryFilter = "");
    bool addFaq(const Faq& f);
    bool updateFaq(const Faq& f);
    bool deleteFaq(const QString& id);
    void incrementFaqView(const QString& id);

    // Synonyms
    QList<Synonym> getAllSynonyms();
    bool addSynonym(const QString& primaryWord, const QStringList& synonyms);

    // Chat Messages & Feedback
    QString saveChatMessage(const QString& sessionId, const QString& role, const QString& content, const QString& refEntityJson = "");
    bool recordFeedback(const QString& messageId, bool helpful, const QString& comment = "");
    QList<ChatMessageRecord> getRecentChatMessages(const QString& sessionId, int limit = 30);
    QMap<QString, int> getChatbotAnalytics();

    // Settings
    QString getSetting(const QString& key, const QString& defaultValue = "");
    void setSetting(const QString& key, const QString& value);

    // Admin Auth
    bool verifyAdmin(const QString& username, const QString& password);

private:
    DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
