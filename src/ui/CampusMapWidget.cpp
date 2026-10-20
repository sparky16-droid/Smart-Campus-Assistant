#include "CampusMapWidget.h"
#include "../database/DatabaseManager.h"
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>

CampusMapCanvas::CampusMapCanvas(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setMinimumSize(600, 500);
}

void CampusMapCanvas::setBuildings(const QList<Building>& buildings) {
    m_buildings = buildings;
    update();
}

void CampusMapCanvas::selectBuildingById(const QString& id) {
    m_selectedBuildingId = id;
    for (const auto& b : m_buildings) {
        if (b.id == id) {
            emit buildingSelected(b);
            break;
        }
    }
    update();
}

void CampusMapCanvas::setCategoryFilter(const QString& category) {
    m_categoryFilter = category;
    update();
}

void CampusMapCanvas::zoomIn() {
    m_scale = qMin(2.5, m_scale * 1.15);
    update();
}

void CampusMapCanvas::zoomOut() {
    m_scale = qMax(0.5, m_scale / 1.15);
    update();
}

void CampusMapCanvas::resetView() {
    m_scale = 1.0;
    m_offset = QPointF(50, 50);
    update();
}

QRect CampusMapCanvas::getBuildingScreenRect(const Building& b) const {
    int x = static_cast<int>(b.mapX * m_scale + m_offset.x());
    int y = static_cast<int>(b.mapY * m_scale + m_offset.y());
    int w = static_cast<int>(b.mapWidth * m_scale);
    int h = static_cast<int>(b.mapHeight * m_scale);
    return QRect(x, y, w, h);
}

void CampusMapCanvas::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 1. Campus ground background
    p.fillRect(rect(), QColor("#f8fafc"));

    // Campus boundary
    QRect campusArea(static_cast<int>(20 * m_scale + m_offset.x()),
                     static_cast<int>(20 * m_scale + m_offset.y()),
                     static_cast<int>(740 * m_scale),
                     static_cast<int>(440 * m_scale));

    p.setPen(QPen(QColor("#cbd5e1"), 2, Qt::DashLine));
    p.setBrush(QColor("#f1f5f9"));
    p.drawRoundedRect(campusArea, 12, 12);

    // 2. Central Lawn / Green Zone
    QRect centralLawn(static_cast<int>(280 * m_scale + m_offset.x()),
                      static_cast<int>(180 * m_scale + m_offset.y()),
                      static_cast<int>(100 * m_scale),
                      static_cast<int>(80 * m_scale));
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#dcfce7"));
    p.drawRoundedRect(centralLawn, 16, 16);

    p.setPen(QColor("#15803d"));
    QFont smallFont("Segoe UI", static_cast<int>(9 * m_scale));
    p.setFont(smallFont);
    p.drawText(centralLawn, Qt::AlignCenter, "🌿 Central Lawn\n& Quadrangle");

    // 3. Walkways / Internal Avenues
    p.setPen(QPen(QColor("#e2e8f0"), static_cast<int>(14 * m_scale), Qt::SolidLine, Qt::RoundCap));
    // Horizontal main avenue
    p.drawLine(QPointF(30 * m_scale + m_offset.x(), 260 * m_scale + m_offset.y()),
               QPointF(720 * m_scale + m_offset.x(), 260 * m_scale + m_offset.y()));
    // Vertical avenue
    p.drawLine(QPointF(360 * m_scale + m_offset.x(), 30 * m_scale + m_offset.y()),
               QPointF(360 * m_scale + m_offset.x(), 420 * m_scale + m_offset.y()));

    // 4. Gates labels
    p.setPen(QColor("#64748b"));
    QFont gateFont("Segoe UI", static_cast<int>(10 * m_scale), QFont::Bold);
    p.setFont(gateFont);
    p.drawText(static_cast<int>(30 * m_scale + m_offset.x()),
               static_cast<int>(40 * m_scale + m_offset.y()),
               "🚪 Main Gate 1 (North)");
    p.drawText(static_cast<int>(590 * m_scale + m_offset.x()),
               static_cast<int>(430 * m_scale + m_offset.y()),
               "🚪 Campus Gate 2 (East)");

    // 5. Draw Buildings
    for (const auto& b : m_buildings) {
        QRect bRect = getBuildingScreenRect(b);
        bool isSelected = (b.id == m_selectedBuildingId);
        bool isHovered = (b.id == m_hoveredBuildingId);

        QColor baseColor(b.color.isEmpty() ? "#1e40af" : b.color);
        if (isHovered) baseColor = baseColor.lighter(115);

        // Building Shadow
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, isSelected ? 40 : 20));
        p.drawRoundedRect(bRect.adjusted(3, 3, 3, 3), 8, 8);

        // Building Card
        p.setBrush(baseColor);
        if (isSelected) {
            p.setPen(QPen(QColor("#10b981"), 3)); // Glowing emerald border
        } else if (isHovered) {
            p.setPen(QPen(QColor("#38bdf8"), 2));
        } else {
            p.setPen(QPen(baseColor.darker(120), 1));
        }
        p.drawRoundedRect(bRect, 8, 8);

        // Text details inside building
        p.setPen(Qt::white);
        QFont codeFont("Segoe UI", qMax(8, static_cast<int>(10 * m_scale)), QFont::Bold);
        p.setFont(codeFont);

        // Code badge
        QRect codeRect(bRect.x() + 8, bRect.y() + 6, bRect.width() - 16, static_cast<int>(16 * m_scale));
        p.drawText(codeRect, Qt::AlignLeft | Qt::AlignVCenter, "[" + b.code + "]");

        // Building Name
        QFont titleFont("Segoe UI", qMax(7, static_cast<int>(8 * m_scale)));
        p.setFont(titleFont);
        QRect titleRect(bRect.x() + 8, bRect.y() + static_cast<int>(24 * m_scale), bRect.width() - 16, bRect.height() - static_cast<int>(40 * m_scale));
        p.drawText(titleRect, Qt::AlignLeft | Qt::TextWordWrap, b.name);

        // Floors tag
        QFont subFont("Segoe UI", qMax(6, static_cast<int>(7 * m_scale)));
        p.setFont(subFont);
        QRect flRect(bRect.x() + 8, bRect.bottom() - static_cast<int>(16 * m_scale), bRect.width() - 16, static_cast<int>(14 * m_scale));
        p.drawText(flRect, Qt::AlignRight | Qt::AlignVCenter, QString("%1 Floors").arg(b.floors));
    }
}

void CampusMapCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        for (const auto& b : m_buildings) {
            QRect bRect = getBuildingScreenRect(b);
            if (bRect.contains(event->pos())) {
                m_selectedBuildingId = b.id;
                emit buildingSelected(b);
                update();
                return;
            }
        }
        // Pan start if clicked background
        m_isPanning = true;
        m_lastMousePos = event->pos();
    }
}

void CampusMapCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_offset += QPointF(delta.x(), delta.y());
        m_lastMousePos = event->pos();
        update();
        return;
    }

    QString foundHover = "";
    for (const auto& b : m_buildings) {
        QRect bRect = getBuildingScreenRect(b);
        if (bRect.contains(event->pos())) {
            foundHover = b.id;
            break;
        }
    }
    if (foundHover != m_hoveredBuildingId) {
        m_hoveredBuildingId = foundHover;
        setCursor(foundHover.isEmpty() ? Qt::ArrowCursor : Qt::PointingHandCursor);
        update();
    }
}

void CampusMapCanvas::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}

// CAMPUS MAP WIDGET
CampusMapWidget::CampusMapWidget(QWidget* parent) : QWidget(parent) {
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // Left container: Controls and Canvas
    auto leftContainer = new QWidget(this);
    auto leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);

    // Top map controls bar
    auto controlsLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("🔍 Search building, block, or facility on map...");
    m_searchEdit->setClearButtonEnabled(true);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &CampusMapWidget::onSearchChanged);

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItems({"All Categories", "Academic", "Library", "Cafeteria", "Sports", "Lab", "Hostel", "Administration", "Medical"});
    connect(m_categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CampusMapWidget::onCategoryChanged);

    auto btnZoomIn = new QPushButton("+", this);
    btnZoomIn->setFixedSize(36, 36);
    btnZoomIn->setToolTip("Zoom In");

    auto btnZoomOut = new QPushButton("-", this);
    btnZoomOut->setFixedSize(36, 36);
    btnZoomOut->setToolTip("Zoom Out");

    auto btnReset = new QPushButton("⟲ Reset", this);
    btnReset->setObjectName("btnSecondary");

    controlsLayout->addWidget(m_searchEdit, 2);
    controlsLayout->addWidget(m_categoryCombo, 1);
    controlsLayout->addWidget(btnZoomIn);
    controlsLayout->addWidget(btnZoomOut);
    controlsLayout->addWidget(btnReset);

    // Canvas
    m_canvas = new CampusMapCanvas(this);
    connect(btnZoomIn, &QPushButton::clicked, m_canvas, &CampusMapCanvas::zoomIn);
    connect(btnZoomOut, &QPushButton::clicked, m_canvas, &CampusMapCanvas::zoomOut);
    connect(btnReset, &QPushButton::clicked, m_canvas, &CampusMapCanvas::resetView);
    connect(m_canvas, &CampusMapCanvas::buildingSelected, this, &CampusMapWidget::onBuildingSelected);

    leftLayout->addLayout(controlsLayout);
    leftLayout->addWidget(m_canvas, 1);

    // Right side: Detail Drawer
    m_detailCard = new QFrame(this);
    m_detailCard->setObjectName("cardFrame");
    m_detailCard->setFixedWidth(320);

    auto detailLayout = new QVBoxLayout(m_detailCard);
    detailLayout->setContentsMargins(18, 18, 18, 18);
    detailLayout->setSpacing(10);

    m_detailTitle = new QLabel("Select a Building", m_detailCard);
    m_detailTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #0f172a;");
    m_detailTitle->setWordWrap(true);

    m_detailSubtitle = new QLabel("Click any block on the map to inspect", m_detailCard);
    m_detailSubtitle->setStyleSheet("color: #64748b; font-size: 12px;");

    m_detailDescription = new QLabel("", m_detailCard);
    m_detailDescription->setWordWrap(true);
    m_detailDescription->setStyleSheet("color: #334155; line-height: 1.4;");

    m_detailSpecs = new QLabel("", m_detailCard);
    m_detailSpecs->setStyleSheet("color: #475569; font-size: 12px; background: #f1f5f9; padding: 6px; border-radius: 6px;");

    auto lblFacilities = new QLabel("Facilities in this Building:", m_detailCard);
    lblFacilities->setStyleSheet("font-weight: 600; color: #1e293b; margin-top: 8px;");

    m_facilityList = new QListWidget(m_detailCard);
    m_facilityList->setStyleSheet("border: 1px solid #e2e8f0; border-radius: 6px; background: #f8fafc;");

    m_btnAskAssistant = new QPushButton("💬 Ask Assistant about this Place", m_detailCard);
    m_btnAskAssistant->setObjectName("btnAccent");
    m_btnAskAssistant->setEnabled(false);

    connect(m_btnAskAssistant, &QPushButton::clicked, [this]() {
        if (!m_currentBuilding.name.isEmpty()) {
            emit askAssistantAboutPlace(m_currentBuilding.name);
        }
    });

    detailLayout->addWidget(m_detailTitle);
    detailLayout->addWidget(m_detailSubtitle);
    detailLayout->addWidget(m_detailDescription);
    detailLayout->addWidget(m_detailSpecs);
    detailLayout->addWidget(lblFacilities);
    detailLayout->addWidget(m_facilityList, 1);
    detailLayout->addWidget(m_btnAskAssistant);

    mainLayout->addWidget(leftContainer, 1);
    mainLayout->addWidget(m_detailCard);

    refreshData();
}

void CampusMapWidget::refreshData() {
    QList<Building> buildings = DatabaseManager::instance().getAllBuildings();
    m_canvas->setBuildings(buildings);
    if (!buildings.isEmpty() && m_currentBuilding.id.isEmpty()) {
        onBuildingSelected(buildings.first());
    }
}

void CampusMapWidget::highlightBuilding(const QString& buildingId) {
    m_canvas->selectBuildingById(buildingId);
}

void CampusMapWidget::onBuildingSelected(const Building& b) {
    m_currentBuilding = b;
    m_detailTitle->setText(b.name);
    m_detailSubtitle->setText(QString("Code: %1 • Year Built: %2").arg(b.code).arg(b.yearBuilt));
    m_detailDescription->setText(b.description);
    m_detailSpecs->setText(QString("🏢 Floors: %1  |  ♿ Wheelchair Accessible: %2")
                           .arg(b.floors)
                           .arg(b.wheelchairAccessible ? "Yes" : "No"));

    m_facilityList->clear();
    QList<Facility> facs = DatabaseManager::instance().getAllFacilities();
    for (const auto& f : facs) {
        if (f.buildingId == b.id) {
            QString itemText = QString("• %1 (%2)").arg(f.name).arg(f.category);
            if (!f.floor.isEmpty()) itemText += " - " + f.floor;
            m_facilityList->addItem(itemText);
        }
    }
    if (m_facilityList->count() == 0) {
        m_facilityList->addItem("No sub-facilities listed.");
    }

    m_btnAskAssistant->setEnabled(true);
}

void CampusMapWidget::onSearchChanged(const QString& text) {
    if (text.trimmed().isEmpty()) return;
    QList<Building> buildings = DatabaseManager::instance().getAllBuildings();
    for (const auto& b : buildings) {
        if (b.name.contains(text, Qt::CaseInsensitive) || b.code.contains(text, Qt::CaseInsensitive)) {
            m_canvas->selectBuildingById(b.id);
            break;
        }
    }
}

void CampusMapWidget::onCategoryChanged(int) {
    QString cat = m_categoryCombo->currentText();
    m_canvas->setCategoryFilter(cat == "All Categories" ? "" : cat);
}
