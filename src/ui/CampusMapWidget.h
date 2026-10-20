#ifndef CAMPUSMAPWIDGET_H
#define CAMPUSMAPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QListWidget>
#include "../database/Models.h"

class CampusMapCanvas : public QWidget {
    Q_OBJECT

public:
    explicit CampusMapCanvas(QWidget* parent = nullptr);

    void setBuildings(const QList<Building>& buildings);
    void selectBuildingById(const QString& id);
    void setCategoryFilter(const QString& category);
    void zoomIn();
    void zoomOut();
    void resetView();

signals:
    void buildingSelected(const Building& b);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QList<Building> m_buildings;
    QString m_selectedBuildingId;
    QString m_hoveredBuildingId;
    QString m_categoryFilter;

    double m_scale = 1.0;
    QPointF m_offset = QPointF(50, 50);
    QPoint m_lastMousePos;
    bool m_isPanning = false;

    QRect getBuildingScreenRect(const Building& b) const;
};

class CampusMapWidget : public QWidget {
    Q_OBJECT

public:
    explicit CampusMapWidget(QWidget* parent = nullptr);
    void refreshData();
    void highlightBuilding(const QString& buildingId);

signals:
    void askAssistantAboutPlace(const QString& placeName);

private slots:
    void onBuildingSelected(const Building& b);
    void onSearchChanged(const QString& text);
    void onCategoryChanged(int index);

private:
    CampusMapCanvas* m_canvas;
    QLineEdit* m_searchEdit;
    QComboBox* m_categoryCombo;

    // Side detail drawer
    QFrame* m_detailCard;
    QLabel* m_detailTitle;
    QLabel* m_detailSubtitle;
    QLabel* m_detailDescription;
    QLabel* m_detailSpecs;
    QListWidget* m_facilityList;
    QPushButton* m_btnAskAssistant;
    Building m_currentBuilding;
};

#endif // CAMPUSMAPWIDGET_H
