#include "Observer.h"
#include <algorithm>
#include <iostream>
#include <sstream>

void CampusNoticePublisher::attach(std::shared_ptr<INoticeObserver> observer) {
    if (!observer) return;
    for (const auto& obs : m_observers) {
        if (obs->getObserverId() == observer->getObserverId()) {
            return; // Already attached
        }
    }
    m_observers.push_back(observer);
}

void CampusNoticePublisher::detach(const std::string& observerId) {
    auto it = std::remove_if(m_observers.begin(), m_observers.end(), [&observerId](const std::shared_ptr<INoticeObserver>& obs) {
        return obs->getObserverId() == observerId;
    });
    if (it != m_observers.end()) {
        m_observers.erase(it, m_observers.end());
    }
}

void CampusNoticePublisher::notify(const NoticeEntity& notice) {
    for (auto& obs : m_observers) {
        obs->onNoticePublished(notice);
    }
}

StudentTerminalNotifier::StudentTerminalNotifier(std::string studentId, std::string department)
    : m_studentId(std::move(studentId)), m_department(std::move(department))
{
}

void StudentTerminalNotifier::onNoticePublished(const NoticeEntity& notice) {
    std::cout << "\n>>> [REAL-TIME ALERT FOR " << m_studentId << " (" << m_department << ")] <<<\n"
              << "New Notice Posted: [" << notice.getPriorityString() << "] " 
              << notice.getName() << "\nCategory: " << notice.getCategory() 
              << "\nIssued By: " << notice.getAuthorName() << "\n";
}

AuditLoggerObserver::AuditLoggerObserver(std::string logPrefix)
    : m_logPrefix(std::move(logPrefix))
{
}

void AuditLoggerObserver::onNoticePublished(const NoticeEntity& notice) {
    std::stringstream ss;
    ss << "[" << m_logPrefix << "] Notice Dispatched: ID=" << notice.getId() 
       << ", Priority=" << notice.getPriorityString() 
       << ", Title=\"" << notice.getName() << "\"";
    m_auditLogs.push_back(ss.str());
}
