#ifndef OBSERVER_H
#define OBSERVER_H

#include "NoticeEntity.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @file Observer.h
 * @brief Implementation of Observer Pattern for real-time campus notice dissemination.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: OBSERVER DESIGN PATTERN
 * - Defines a one-to-many dependency between objects.
 * - When a new urgent notice is published, all subscribed observers (students, staff, kiosks)
 *   are notified automatically.
 */
class INoticeObserver {
public:
    virtual ~INoticeObserver() = default;
    virtual void onNoticePublished(const NoticeEntity& notice) = 0;
    virtual std::string getObserverId() const = 0;
};

class INoticeSubject {
public:
    virtual ~INoticeSubject() = default;
    virtual void attach(std::shared_ptr<INoticeObserver> observer) = 0;
    virtual void detach(const std::string& observerId) = 0;
    virtual void notify(const NoticeEntity& notice) = 0;
};

class CampusNoticePublisher : public INoticeSubject {
private:
    std::vector<std::shared_ptr<INoticeObserver>> m_observers;

public:
    CampusNoticePublisher() = default;
    virtual ~CampusNoticePublisher() override = default;

    void attach(std::shared_ptr<INoticeObserver> observer) override;
    void detach(const std::string& observerId) override;
    void notify(const NoticeEntity& notice) override;
    size_t getSubscriberCount() const { return m_observers.size(); }
};

class StudentTerminalNotifier : public INoticeObserver {
private:
    std::string m_studentId;
    std::string m_department;

public:
    StudentTerminalNotifier(std::string studentId, std::string department);
    virtual ~StudentTerminalNotifier() override = default;

    void onNoticePublished(const NoticeEntity& notice) override;
    std::string getObserverId() const override { return m_studentId; }
};

class AuditLoggerObserver : public INoticeObserver {
private:
    std::string m_logPrefix;
    std::vector<std::string> m_auditLogs;

public:
    explicit AuditLoggerObserver(std::string logPrefix);
    virtual ~AuditLoggerObserver() override = default;

    void onNoticePublished(const NoticeEntity& notice) override;
    std::string getObserverId() const override { return "audit_logger"; }
    const std::vector<std::string>& getLogs() const { return m_auditLogs; }
};

#endif // OBSERVER_H
