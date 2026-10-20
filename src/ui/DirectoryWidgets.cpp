#include "DirectoryWidgets.h"
#include "../database/DatabaseManager.h"
#include <QFrame>

// ==========================================
// 1. DEPARTMENTS WIDGET
// ==========================================
DepartmentsWidget::DepartmentsWidget(QWidget* parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(16);

    auto lblTitle = new QLabel("Academic Departments", this);
    lblTitle->setStyleSheet("font-size: 20px; font-weight: 700; color: #0f172a;");
    auto lblSub = new QLabel("Explore engineering, computing, management programs and department leadership", this);
    lblSub->setStyleSheet("color: #64748b; font-size: 13px;");

    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblSub);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    m_cardsContainer = new QWidget(scrollArea);
    m_cardsLayout = new QVBoxLayout(m_cardsContainer);
    m_cardsLayout->setContentsMargins(0, 0, 0, 0);
    m_cardsLayout->setSpacing(14);
    m_cardsLayout->addStretch();

    scrollArea->setWidget(m_cardsContainer);
    mainLayout->addWidget(scrollArea, 1);

    refreshData();
}

void DepartmentsWidget::refreshData() {
    // Clear old cards
    while (m_cardsLayout->count() > 1) {
        QLayoutItem* item = m_cardsLayout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }

    QList<Department> depts = DatabaseManager::instance().getAllDepartments();
    for (const auto& d : depts) {
        auto card = new QFrame(m_cardsContainer);
        card->setObjectName("cardFrame");
        auto cl = new QVBoxLayout(card);
        cl->setContentsMargins(18, 16, 18, 16);
        cl->setSpacing(8);

        // Header: Name and Code
        auto hl = new QHBoxLayout();
        auto lblName = new QLabel(QString("Department of %1 (%2)").arg(d.name).arg(d.code), card);
        lblName->setStyleSheet("font-size: 16px; font-weight: 700; color: #1e40af;");

        auto btnMap = new QPushButton("🗺️ Show on Map", card);
        btnMap->setObjectName("btnSecondary");
        btnMap->setStyleSheet("font-size: 11px; padding: 4px 10px;");
        QString bldId = d.buildingId;
        connect(btnMap, &QPushButton::clicked, [this, bldId]() {
            emit locateDepartmentOnMap(bldId);
        });

        hl->addWidget(lblName, 1);
        hl->addWidget(btnMap);
        cl->addLayout(hl);

        // Details
        auto lblHod = new QLabel(QString("<b>Head of Department:</b> %1  |  <b>Building:</b> %2")
                                 .arg(d.headOfDepartment).arg(d.buildingName.isEmpty() ? "Academic Block" : d.buildingName), card);
        lblHod->setStyleSheet("color: #334155; font-size: 13px;");

        auto lblContact = new QLabel(QString("✉️ %1 %2")
                                     .arg(d.contactEmail)
                                     .arg(d.contactPhone.isEmpty() ? "" : "  |  📞 " + d.contactPhone), card);
        lblContact->setStyleSheet("color: #64748b; font-size: 12px;");

        auto lblPrograms = new QLabel(QString("<b>Programs:</b> %1").arg(d.programsOffered), card);
        lblPrograms->setStyleSheet("color: #0d9488; font-size: 12px; font-weight: 500;");

        auto lblDesc = new QLabel(d.description, card);
        lblDesc->setStyleSheet("color: #475569; font-size: 12px; line-height: 1.4;");
        lblDesc->setWordWrap(true);

        cl->addWidget(lblHod);
        cl->addWidget(lblContact);
        cl->addWidget(lblPrograms);
        cl->addWidget(lblDesc);

        m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
    }
}

// ==========================================
// 2. FACULTY DIRECTORY WIDGET
// ==========================================
FacultyWidget::FacultyWidget(QWidget* parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(14);

    auto lblTitle = new QLabel("Faculty Directory", this);
    lblTitle->setStyleSheet("font-size: 20px; font-weight: 700; color: #0f172a;");
    auto lblSub = new QLabel("Find faculty contact info, cabin locations, office consultation hours, and research interests", this);
    lblSub->setStyleSheet("color: #64748b; font-size: 13px;");

    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblSub);

    // Search & Filter controls
    auto filterBar = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("🔍 Search faculty by name, specialization, or designation...");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &FacultyWidget::applyFilter);

    m_deptCombo = new QComboBox(this);
    m_deptCombo->addItem("All Departments", "");
    QList<Department> depts = DatabaseManager::instance().getAllDepartments();
    for (const auto& d : depts) {
        m_deptCombo->addItem(d.name, d.id);
    }
    connect(m_deptCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FacultyWidget::applyFilter);

    filterBar->addWidget(m_searchEdit, 2);
    filterBar->addWidget(m_deptCombo, 1);
    mainLayout->addLayout(filterBar);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    m_cardsContainer = new QWidget(scrollArea);
    m_cardsLayout = new QVBoxLayout(m_cardsContainer);
    m_cardsLayout->setContentsMargins(0, 0, 0, 0);
    m_cardsLayout->setSpacing(12);
    m_cardsLayout->addStretch();

    scrollArea->setWidget(m_cardsContainer);
    mainLayout->addWidget(scrollArea, 1);

    refreshData();
}

void FacultyWidget::refreshData() {
    m_facultyList = DatabaseManager::instance().getAllFaculty();
    applyFilter();
}

void FacultyWidget::applyFilter() {
    while (m_cardsLayout->count() > 1) {
        QLayoutItem* item = m_cardsLayout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }

    QString search = m_searchEdit->text().trimmed();
    QString selectedDeptId = m_deptCombo->currentData().toString();

    for (const auto& f : m_facultyList) {
        if (!selectedDeptId.isEmpty() && f.departmentId != selectedDeptId) continue;
        if (!search.isEmpty()) {
            bool match = f.name.contains(search, Qt::CaseInsensitive) ||
                         f.designation.contains(search, Qt::CaseInsensitive) ||
                         f.specialization.contains(search, Qt::CaseInsensitive) ||
                         f.departmentName.contains(search, Qt::CaseInsensitive);
            if (!match) continue;
        }

        auto card = new QFrame(m_cardsContainer);
        card->setObjectName("cardFrame");
        auto cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 14, 16, 14);
        cl->setSpacing(6);

        auto hl = new QHBoxLayout();
        auto lblName = new QLabel(QString("👨‍🏫 %1 %2").arg(f.title).arg(f.name), card);
        lblName->setStyleSheet("font-size: 15px; font-weight: 700; color: #0f172a;");

        auto lblDesig = new QLabel(QString("%1 • %2").arg(f.designation).arg(f.departmentName), card);
        lblDesig->setStyleSheet("color: #1e40af; font-size: 12px; font-weight: 600;");

        hl->addWidget(lblName);
        hl->addSpacing(10);
        hl->addWidget(lblDesig, 1);
        cl->addLayout(hl);

        auto lblMeta = new QLabel(QString("🚪 <b>Cabin:</b> %1   |   ⏰ <b>Office Hours:</b> %2")
                                  .arg(f.cabinRoom.isEmpty() ? "Main Faculty Room" : f.cabinRoom)
                                  .arg(f.officeHours.isEmpty() ? "By Appointment" : f.officeHours), card);
        lblMeta->setStyleSheet("color: #334155; font-size: 12px;");

        auto lblContact = new QLabel(QString("✉️ %1 %2")
                                     .arg(f.email)
                                     .arg(f.phone.isEmpty() ? "" : "  |  📞 " + f.phone), card);
        lblContact->setStyleSheet("color: #64748b; font-size: 12px;");

        cl->addWidget(lblMeta);
        cl->addWidget(lblContact);

        if (!f.specialization.isEmpty()) {
            auto lblSpec = new QLabel(QString("<b>Research / Specialization:</b> %1").arg(f.specialization), card);
            lblSpec->setStyleSheet("color: #0d9488; font-size: 12px;");
            cl->addWidget(lblSpec);
        }

        if (!f.bio.isEmpty()) {
            auto lblBio = new QLabel(QString("<i>%1</i>").arg(f.bio), card);
            lblBio->setStyleSheet("color: #475569; font-size: 11px;");
            lblBio->setWordWrap(true);
            cl->addWidget(lblBio);
        }

        m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
    }
}

// ==========================================
// 3. FACILITIES AND SERVICES WIDGET
// ==========================================
FacilitiesWidget::FacilitiesWidget(QWidget* parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(14);

    auto lblTitle = new QLabel("Campus Facilities & Amenities", this);
    lblTitle->setStyleSheet("font-size: 20px; font-weight: 700; color: #0f172a;");
    auto lblSub = new QLabel("Discover dining halls, computing clusters, clinics, athletics pavilion, ATMs, and administrative desks", this);
    lblSub->setStyleSheet("color: #64748b; font-size: 13px;");

    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblSub);

    // Category filter pills bar
    m_categoryBar = new QHBoxLayout();
    m_categoryBar->setSpacing(8);

    QStringList categories = {"All", "Library", "Cafeteria", "Lab", "Administration", "Medical", "Sports", "Hostel", "ATM", "Transport"};
    for (const auto& cat : categories) {
        auto btnCat = new QPushButton(cat, this);
        btnCat->setObjectName(cat == "All" ? "chipBtn" : "btnSecondary");
        btnCat->setStyleSheet("font-size: 12px; padding: 5px 14px; border-radius: 14px;");
        connect(btnCat, &QPushButton::clicked, [this, cat]() {
            onCategorySelected(cat);
        });
        m_categoryBar->addWidget(btnCat);
    }
    m_categoryBar->addStretch();
    mainLayout->addLayout(m_categoryBar);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    m_cardsContainer = new QWidget(scrollArea);
    m_cardsLayout = new QVBoxLayout(m_cardsContainer);
    m_cardsLayout->setContentsMargins(0, 0, 0, 0);
    m_cardsLayout->setSpacing(12);
    m_cardsLayout->addStretch();

    scrollArea->setWidget(m_cardsContainer);
    mainLayout->addWidget(scrollArea, 1);

    m_selectedCategory = "All";
    refreshData();
}

void FacilitiesWidget::onCategorySelected(const QString& category) {
    m_selectedCategory = category;
    refreshData();
}

void FacilitiesWidget::refreshData() {
    while (m_cardsLayout->count() > 1) {
        QLayoutItem* item = m_cardsLayout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }

    QList<Facility> facs = DatabaseManager::instance().getAllFacilities(m_selectedCategory);
    for (const auto& f : facs) {
        auto card = new QFrame(m_cardsContainer);
        card->setObjectName("cardFrame");
        auto cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 14, 16, 14);
        cl->setSpacing(6);

        auto hl = new QHBoxLayout();
        auto lblName = new QLabel(QString("🏢 %1").arg(f.name), card);
        lblName->setStyleSheet("font-size: 15px; font-weight: 700; color: #0f172a;");

        auto lblCat = new QLabel(f.category, card);
        lblCat->setStyleSheet("background: #e0f2fe; color: #0369a1; padding: 3px 8px; border-radius: 6px; font-size: 11px; font-weight: 700;");

        auto btnMap = new QPushButton("🗺️ Show on Map", card);
        btnMap->setObjectName("btnSecondary");
        btnMap->setStyleSheet("font-size: 11px; padding: 3px 8px;");
        QString bldId = f.buildingId;
        connect(btnMap, &QPushButton::clicked, [this, bldId]() {
            emit locateFacilityOnMap(bldId);
        });

        hl->addWidget(lblName);
        hl->addWidget(lblCat);
        hl->addStretch();
        hl->addWidget(btnMap);
        cl->addLayout(hl);

        auto lblLoc = new QLabel(QString("📍 <b>Building:</b> %1%2%3")
                                 .arg(f.buildingName.isEmpty() ? "Campus Grounds" : f.buildingName)
                                 .arg(f.floor.isEmpty() ? "" : QString(" (%1)").arg(f.floor))
                                 .arg(f.roomNumber.isEmpty() ? "" : QString(" - Room %1").arg(f.roomNumber)), card);
        lblLoc->setStyleSheet("color: #334155; font-size: 12px;");

        if (!f.landmark.isEmpty()) {
            auto lblLandmark = new QLabel(QString("🧭 <b>Landmark:</b> %1").arg(f.landmark), card);
            lblLandmark->setStyleSheet("color: #64748b; font-size: 12px;");
            cl->addWidget(lblLandmark);
        }

        if (!f.timings.isEmpty()) {
            auto lblTime = new QLabel(QString("⏰ <b>Operating Hours:</b> %1").arg(f.timings), card);
            lblTime->setStyleSheet("color: #059669; font-size: 12px; font-weight: 600;");
            cl->addWidget(lblTime);
        }

        auto lblDesc = new QLabel(f.description, card);
        lblDesc->setStyleSheet("color: #475569; font-size: 12px;");
        lblDesc->setWordWrap(true);

        cl->addWidget(lblLoc);
        cl->addWidget(lblDesc);

        m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
    }
}
