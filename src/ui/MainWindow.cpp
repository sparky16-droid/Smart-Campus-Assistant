#include "MainWindow.h"
#include "ThemeHelper.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QDesktopServices>
#include <QUrl>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Smart Campus Assistant - AI-Assisted Navigation & Campus Intelligence");
    resize(1200, 780);
    setMinimumSize(950, 600);

    // Apply global modern stylesheet
    setStyleSheet(ThemeHelper::getAppStyleSheet());

    auto central = new QWidget(this);
    setCentralWidget(central);

    auto mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupSidebar();
    setupPages();

    mainLayout->addWidget(m_pagesStack, 1);

    // Start embedded C++ HTTP server
    m_webServer = new LocalWebServer(8080, this);
    m_webServer->start();

    // Status bar
    statusBar()->setStyleSheet("background: #0f172a; color: #94a3b8; font-size: 11px; padding: 4px;");
    statusBar()->showMessage("🟢 Connected to SQLite Database: smart_campus.db  |  🌐 Embedded Web Server: http://localhost:8080  |  Demo Mode Active");

    switchPage(0);
}

MainWindow::~MainWindow() {
    if (m_webServer) {
        m_webServer->stop();
    }
}

void MainWindow::setupSidebar() {
    auto sidebar = new QFrame(this);
    sidebar->setObjectName("navSidebar");
    sidebar->setFixedWidth(230);

    auto sl = new QVBoxLayout(sidebar);
    sl->setContentsMargins(14, 20, 14, 20);
    sl->setSpacing(6);

    // Branding logo & title
    auto brandLayout = new QVBoxLayout();
    brandLayout->setSpacing(2);

    auto lblLogo = new QLabel("🏛️ SMART CAMPUS", sidebar);
    lblLogo->setStyleSheet("font-size: 16px; font-weight: 800; color: #ffffff; letter-spacing: 0.5px;");

    auto lblSubtitle = new QLabel("College Digital Assistant", sidebar);
    lblSubtitle->setStyleSheet("font-size: 11px; color: #38bdf8; font-weight: 600; margin-bottom: 14px;");

    brandLayout->addWidget(lblLogo);
    brandLayout->addWidget(lblSubtitle);
    sl->addLayout(brandLayout);

    // Navigation buttons definition
    struct NavDef {
        QString label;
        int pageIndex;
    };

    QList<NavDef> navItems = {
        {"🏠  Home Portal", 0},
        {"🤖  AI Chat Assistant", 1},
        {"🗺️  Campus Map", 2},
        {"🎓  Departments", 3},
        {"👨‍🏫  Faculty Directory", 4},
        {"🏢  Facilities & Services", 5},
        {"📢  Notices & Circulars", 6},
        {"❓  Frequently Asked FAQs", 7},
        {"ℹ️  About & Contact", 8}
    };

    for (const auto& item : navItems) {
        auto btn = new QPushButton(item.label, sidebar);
        btn->setObjectName("navBtn");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setCheckable(true);

        int idx = item.pageIndex;
        connect(btn, &QPushButton::clicked, [this, idx]() {
            switchPage(idx);
        });

        m_navButtons.append(btn);
        sl->addWidget(btn);
    }

    sl->addStretch();

    // Embedded Web Server Shortcut
    auto btnBrowser = new QPushButton("🌐 Open in Web Browser", sidebar);
    btnBrowser->setObjectName("btnAccent");
    btnBrowser->setStyleSheet("font-size: 12px; font-weight: 600; padding: 8px 12px; margin-bottom: 6px;");
    connect(btnBrowser, &QPushButton::clicked, this, &MainWindow::onOpenInBrowser);
    sl->addWidget(btnBrowser);

    // Admin Portal button
    auto btnAdmin = new QPushButton("🔐 Administrator Portal", sidebar);
    btnAdmin->setStyleSheet("background: #1e293b; color: #f8fafc; font-size: 12px; padding: 8px 12px; border-radius: 6px; font-weight: 600;");
    connect(btnAdmin, &QPushButton::clicked, this, &MainWindow::onOpenAdmin);
    sl->addWidget(btnAdmin);

    // Add sidebar to main layout
    auto central = centralWidget();
    auto l = qobject_cast<QHBoxLayout*>(central->layout());
    if (l) {
        l->addWidget(sidebar);
    }
}

void MainWindow::setupPages() {
    m_pagesStack = new QStackedWidget(this);

    m_homePage = new HomeWidget(this);
    m_chatPage = new ChatbotWidget(this);
    m_mapPage = new CampusMapWidget(this);
    m_deptsPage = new DepartmentsWidget(this);
    m_facultyPage = new FacultyWidget(this);
    m_facilitiesPage = new FacilitiesWidget(this);
    m_noticesPage = new NoticesWidget(this);
    m_faqsPage = new FaqWidget(this);
    m_aboutPage = new AboutWidget(this);

    // Connect Home signals
    connect(m_homePage, &HomeWidget::navigateToPage, this, &MainWindow::switchPage);
    connect(m_homePage, &HomeWidget::askAssistantQuestion, this, &MainWindow::onAskAssistant);

    // Connect Chatbot signals
    connect(m_chatPage, &ChatbotWidget::navigateToMap, this, &MainWindow::onLocateBuilding);

    // Connect Map signals
    connect(m_mapPage, &CampusMapWidget::askAssistantAboutPlace, this, [this](const QString& place) {
        onAskAssistant("Tell me about " + place + " and its location");
    });

    // Connect Directory signals
    connect(m_deptsPage, &DepartmentsWidget::locateDepartmentOnMap, this, &MainWindow::onLocateBuilding);
    connect(m_facilitiesPage, &FacilitiesWidget::locateFacilityOnMap, this, &MainWindow::onLocateBuilding);

    // Connect FAQs signal
    connect(m_faqsPage, &FaqWidget::askAssistantQuestion, this, &MainWindow::onAskAssistant);

    m_pagesStack->addWidget(m_homePage);        // 0
    m_pagesStack->addWidget(m_chatPage);        // 1
    m_pagesStack->addWidget(m_mapPage);         // 2
    m_pagesStack->addWidget(m_deptsPage);       // 3
    m_pagesStack->addWidget(m_facultyPage);     // 4
    m_pagesStack->addWidget(m_facilitiesPage);  // 5
    m_pagesStack->addWidget(m_noticesPage);     // 6
    m_pagesStack->addWidget(m_faqsPage);        // 7
    m_pagesStack->addWidget(m_aboutPage);       // 8
}

void MainWindow::switchPage(int index) {
    if (index >= 0 && index < m_pagesStack->count()) {
        m_pagesStack->setCurrentIndex(index);

        for (int i = 0; i < m_navButtons.size(); ++i) {
            m_navButtons[i]->setChecked(i == index);
        }
    }
}

void MainWindow::onAskAssistant(const QString& question) {
    switchPage(1); // Switch to Chatbot tab
    m_chatPage->sendQuery(question);
}

void MainWindow::onLocateBuilding(const QString& buildingIdOrName) {
    switchPage(2); // Switch to Campus Map tab
    m_mapPage->highlightBuilding(buildingIdOrName);
}

void MainWindow::onOpenAdmin() {
    AdminDialog dlg(this);
    connect(&dlg, &AdminDialog::dataModified, this, &MainWindow::onDataModified);
    dlg.exec();
}

void MainWindow::onOpenInBrowser() {
    QDesktopServices::openUrl(QUrl("http://localhost:8080"));
}

void MainWindow::onDataModified() {
    m_homePage->refreshData();
    m_mapPage->refreshData();
    m_deptsPage->refreshData();
    m_facultyPage->refreshData();
    m_facilitiesPage->refreshData();
    m_noticesPage->refreshData();
    m_faqsPage->refreshData();
}
