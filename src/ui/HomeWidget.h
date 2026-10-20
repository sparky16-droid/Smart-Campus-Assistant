#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>

class HomeWidget : public QWidget {
    Q_OBJECT

public:
    explicit HomeWidget(QWidget* parent = nullptr);
    void refreshData();

signals:
    void navigateToPage(int pageIndex); // 0=Home, 1=Chat, 2=Map, 3=Depts, 4=Faculty, 5=Facilities, 6=Notices, 7=FAQs, 8=About, 9=Admin
    void askAssistantQuestion(const QString& question);

private slots:
    void onSearchTriggered();

private:
    QLineEdit* m_searchEdit;
    QLabel* m_lblCollegeName;
    QLabel* m_lblCollegeTagline;
    QLabel* m_statBuildings;
    QLabel* m_statDepts;
    QLabel* m_statFaculty;
    QLabel* m_statFacilities;
    QVBoxLayout* m_noticesFeedLayout;
    QWidget* m_noticesFeedContainer;
};

#endif // HOMEWIDGET_H
