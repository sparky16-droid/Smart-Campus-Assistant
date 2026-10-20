#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QList>
#include "HomeWidget.h"
#include "ChatbotWidget.h"
#include "CampusMapWidget.h"
#include "DirectoryWidgets.h"
#include "NoticesFaqWidgets.h"
#include "AdminDialog.h"
#include "../server/LocalWebServer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void switchPage(int index);
    void onAskAssistant(const QString& question);
    void onLocateBuilding(const QString& buildingId);
    void onOpenAdmin();
    void onOpenInBrowser();
    void onDataModified();

private:
    void setupSidebar();
    void setupPages();

    QStackedWidget* m_pagesStack;
    QList<QPushButton*> m_navButtons;

    HomeWidget* m_homePage;
    ChatbotWidget* m_chatPage;
    CampusMapWidget* m_mapPage;
    DepartmentsWidget* m_deptsPage;
    FacultyWidget* m_facultyPage;
    FacilitiesWidget* m_facilitiesPage;
    NoticesWidget* m_noticesPage;
    FaqWidget* m_faqsPage;
    AboutWidget* m_aboutPage;

    LocalWebServer* m_webServer;
};

#endif // MAINWINDOW_H
