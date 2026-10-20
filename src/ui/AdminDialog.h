#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

class AdminDialog : public QDialog {
    Q_OBJECT

public:
    explicit AdminDialog(QWidget* parent = nullptr);

signals:
    void dataModified();

private slots:
    void onLoginClicked();
    void onLogoutClicked();
    void refreshAnalytics();
    void refreshBuildingsTable();
    void refreshFacilitiesTable();
    void refreshNoticesTable();
    void refreshFaqsTable();

    void onAddBuilding();
    void onDeleteBuilding();
    void onAddFacility();
    void onDeleteFacility();
    void onAddNotice();
    void onDeleteNotice();
    void onAddFaq();
    void onDeleteFaq();
    void onSaveSettings();

private:
    void setupLoginView();
    void setupDashboardView();

    QStackedWidget* m_stack;
    QWidget* m_loginWidget;
    QWidget* m_dashboardWidget;

    // Login controls
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QLabel* m_lblLoginError;

    // Dashboard controls
    QTabWidget* m_tabWidget;
    QLabel* m_statQueries;
    QLabel* m_statHelpful;
    QLabel* m_statRatio;
    QLabel* m_statTotalFacs;

    QTableWidget* m_buildingsTable;
    QTableWidget* m_facilitiesTable;
    QTableWidget* m_noticesTable;
    QTableWidget* m_faqsTable;

    QLineEdit* m_settingCollegeName;
    QLineEdit* m_settingAddress;
    QLineEdit* m_settingPhone;
    QLineEdit* m_settingEmail;
};

#endif // ADMINDIALOG_H
