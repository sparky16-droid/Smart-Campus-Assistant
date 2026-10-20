#ifndef DIRECTORYWIDGETS_H
#define DIRECTORYWIDGETS_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "../database/Models.h"

// DEPARTMENTS WIDGET
class DepartmentsWidget : public QWidget {
    Q_OBJECT

public:
    explicit DepartmentsWidget(QWidget* parent = nullptr);
    void refreshData();

signals:
    void locateDepartmentOnMap(const QString& buildingId);

private:
    QVBoxLayout* m_cardsLayout;
    QWidget* m_cardsContainer;
};

// FACULTY DIRECTORY WIDGET
class FacultyWidget : public QWidget {
    Q_OBJECT

public:
    explicit FacultyWidget(QWidget* parent = nullptr);
    void refreshData();

private slots:
    void applyFilter();

private:
    QLineEdit* m_searchEdit;
    QComboBox* m_deptCombo;
    QVBoxLayout* m_cardsLayout;
    QWidget* m_cardsContainer;
    QList<Faculty> m_facultyList;
};

// FACILITIES AND SERVICES WIDGET
class FacilitiesWidget : public QWidget {
    Q_OBJECT

public:
    explicit FacilitiesWidget(QWidget* parent = nullptr);
    void refreshData();

signals:
    void locateFacilityOnMap(const QString& buildingId);

private slots:
    void onCategorySelected(const QString& category);

private:
    QString m_selectedCategory;
    QHBoxLayout* m_categoryBar;
    QVBoxLayout* m_cardsLayout;
    QWidget* m_cardsContainer;
};

#endif // DIRECTORYWIDGETS_H
