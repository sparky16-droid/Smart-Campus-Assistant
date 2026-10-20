#include "AdminDialog.h"
#include "../database/DatabaseManager.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>

AdminDialog::AdminDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Administrator Portal - Smart Campus Assistant");
    resize(900, 620);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_stack = new QStackedWidget(this);
    setupLoginView();
    setupDashboardView();

    m_stack->addWidget(m_loginWidget);
    m_stack->addWidget(m_dashboardWidget);

    mainLayout->addWidget(m_stack);
    m_stack->setCurrentWidget(m_loginWidget);
}

void AdminDialog::setupLoginView() {
    m_loginWidget = new QWidget(this);
    auto l = new QVBoxLayout(m_loginWidget);
    l->setAlignment(Qt::AlignCenter);

    auto card = new QFrame(m_loginWidget);
    card->setObjectName("cardFrame");
    card->setFixedWidth(380);
    auto cl = new QVBoxLayout(card);
    cl->setContentsMargins(28, 28, 28, 28);
    cl->setSpacing(14);

    auto lblTitle = new QLabel("🔐 Admin Authentication", card);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: 700; color: #0f172a; text-align: center;");
    lblTitle->setAlignment(Qt::AlignCenter);

    auto lblHint = new QLabel("Demo Login: username <b>admin</b> | password <b>adminpassword123</b>", card);
    lblHint->setStyleSheet("color: #64748b; font-size: 11px; text-align: center; background: #f1f5f9; padding: 6px; border-radius: 6px;");
    lblHint->setWordWrap(true);
    lblHint->setAlignment(Qt::AlignCenter);

    m_usernameEdit = new QLineEdit(card);
    m_usernameEdit->setPlaceholderText("Username (admin)");
    m_usernameEdit->setText("admin");

    m_passwordEdit = new QLineEdit(card);
    m_passwordEdit->setPlaceholderText("Password (adminpassword123)");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setText("adminpassword123");
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &AdminDialog::onLoginClicked);

    m_lblLoginError = new QLabel("", card);
    m_lblLoginError->setStyleSheet("color: #ef4444; font-size: 12px; font-weight: 600;");
    m_lblLoginError->setAlignment(Qt::AlignCenter);

    auto btnLogin = new QPushButton("Log In to Dashboard", card);
    btnLogin->setMinimumHeight(40);
    connect(btnLogin, &QPushButton::clicked, this, &AdminDialog::onLoginClicked);

    cl->addWidget(lblTitle);
    cl->addWidget(lblHint);
    cl->addWidget(new QLabel("Username:", card));
    cl->addWidget(m_usernameEdit);
    cl->addWidget(new QLabel("Password:", card));
    cl->addWidget(m_passwordEdit);
    cl->addWidget(m_lblLoginError);
    cl->addWidget(btnLogin);

    l->addWidget(card);
}

void AdminDialog::setupDashboardView() {
    m_dashboardWidget = new QWidget(this);
    auto l = new QVBoxLayout(m_dashboardWidget);
    l->setContentsMargins(18, 16, 18, 16);
    l->setSpacing(12);

    // Top Header with Logout
    auto headerLayout = new QHBoxLayout();
    auto lblHeader = new QLabel("🛡️ Smart Campus Control Center", m_dashboardWidget);
    lblHeader->setStyleSheet("font-size: 18px; font-weight: 700; color: #0f172a;");

    auto btnLogout = new QPushButton("🚪 Log Out", m_dashboardWidget);
    btnLogout->setObjectName("btnSecondary");
    connect(btnLogout, &QPushButton::clicked, this, &AdminDialog::onLogoutClicked);

    headerLayout->addWidget(lblHeader);
    headerLayout->addStretch();
    headerLayout->addWidget(btnLogout);
    l->addLayout(headerLayout);

    // Tabs
    m_tabWidget = new QTabWidget(m_dashboardWidget);

    // 1. ANALYTICS TAB
    auto tabAnalytics = new QWidget();
    auto al = new QVBoxLayout(tabAnalytics);
    al->setContentsMargins(16, 16, 16, 16);
    al->setSpacing(16);

    auto statsRow = new QHBoxLayout();
    auto makeMetric = [](const QString& label, const QString& color) {
        auto f = new QFrame();
        f->setObjectName("cardFrame");
        auto fl = new QVBoxLayout(f);
        fl->setContentsMargins(14, 12, 14, 12);
        fl->setSpacing(4);
        auto val = new QLabel("0", f);
        val->setStyleSheet(QString("font-size: 22px; font-weight: 800; color: %1;").arg(color));
        auto lbl = new QLabel(label, f);
        lbl->setStyleSheet("color: #64748b; font-size: 12px; font-weight: 600;");
        fl->addWidget(val);
        fl->addWidget(lbl);
        return qMakePair(f, val);
    };

    auto p1 = makeMetric("Total Queries Answered", "#1e40af");
    m_statQueries = p1.second;
    auto p2 = makeMetric("Helpful Feedback 👍", "#059669");
    m_statHelpful = p2.second;
    auto p3 = makeMetric("Satisfaction Rating", "#0d9488");
    m_statRatio = p3.second;
    auto p4 = makeMetric("Campus Facilities Managed", "#d97706");
    m_statTotalFacs = p4.second;

    statsRow->addWidget(p1.first);
    statsRow->addWidget(p2.first);
    statsRow->addWidget(p3.first);
    statsRow->addWidget(p4.first);
    al->addLayout(statsRow);

    auto lblAnalyticsNotice = new QLabel("Metrics are dynamically recorded when users query the AI Chatbot and provide helpfulness feedback in real time.", tabAnalytics);
    lblAnalyticsNotice->setStyleSheet("color: #64748b; font-style: italic; font-size: 12px;");
    al->addWidget(lblAnalyticsNotice);
    al->addStretch();
    m_tabWidget->addTab(tabAnalytics, "📊 Analytics & Feedback");

    // 2. BUILDINGS TAB
    auto tabBuildings = new QWidget();
    auto bl = new QVBoxLayout(tabBuildings);
    bl->setContentsMargins(12, 12, 12, 12);
    m_buildingsTable = new QTableWidget(tabBuildings);
    m_buildingsTable->setColumnCount(5);
    m_buildingsTable->setHorizontalHeaderLabels({"Code", "Building Name", "Floors", "Year", "Wheelchair"});
    m_buildingsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    auto btnRowB = new QHBoxLayout();
    auto btnAddB = new QPushButton("+ Add Building", tabBuildings);
    connect(btnAddB, &QPushButton::clicked, this, &AdminDialog::onAddBuilding);
    auto btnDelB = new QPushButton("🗑️ Delete Selected", tabBuildings);
    btnDelB->setObjectName("btnSecondary");
    connect(btnDelB, &QPushButton::clicked, this, &AdminDialog::onDeleteBuilding);
    btnRowB->addWidget(btnAddB);
    btnRowB->addWidget(btnDelB);
    btnRowB->addStretch();

    bl->addLayout(btnRowB);
    bl->addWidget(m_buildingsTable);
    m_tabWidget->addTab(tabBuildings, "🏛️ Manage Buildings");

    // 3. FACILITIES TAB
    auto tabFacilities = new QWidget();
    auto fl = new QVBoxLayout(tabFacilities);
    fl->setContentsMargins(12, 12, 12, 12);
    m_facilitiesTable = new QTableWidget(tabFacilities);
    m_facilitiesTable->setColumnCount(5);
    m_facilitiesTable->setHorizontalHeaderLabels({"Facility Name", "Category", "Building", "Floor/Room", "Timings"});
    m_facilitiesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    auto btnRowF = new QHBoxLayout();
    auto btnAddF = new QPushButton("+ Add Facility", tabFacilities);
    connect(btnAddF, &QPushButton::clicked, this, &AdminDialog::onAddFacility);
    auto btnDelF = new QPushButton("🗑️ Delete Selected", tabFacilities);
    btnDelF->setObjectName("btnSecondary");
    connect(btnDelF, &QPushButton::clicked, this, &AdminDialog::onDeleteFacility);
    btnRowF->addWidget(btnAddF);
    btnRowF->addWidget(btnDelF);
    btnRowF->addStretch();

    fl->addLayout(btnRowF);
    fl->addWidget(m_facilitiesTable);
    m_tabWidget->addTab(tabFacilities, "🏢 Manage Facilities");

    // 4. NOTICES TAB
    auto tabNotices = new QWidget();
    auto nl = new QVBoxLayout(tabNotices);
    nl->setContentsMargins(12, 12, 12, 12);
    m_noticesTable = new QTableWidget(tabNotices);
    m_noticesTable->setColumnCount(4);
    m_noticesTable->setHorizontalHeaderLabels({"Title", "Category", "Priority", "Author"});
    m_noticesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    auto btnRowN = new QHBoxLayout();
    auto btnAddN = new QPushButton("+ Post New Notice", tabNotices);
    connect(btnAddN, &QPushButton::clicked, this, &AdminDialog::onAddNotice);
    auto btnDelN = new QPushButton("🗑️ Delete Selected", tabNotices);
    btnDelN->setObjectName("btnSecondary");
    connect(btnDelN, &QPushButton::clicked, this, &AdminDialog::onDeleteNotice);
    btnRowN->addWidget(btnAddN);
    btnRowN->addWidget(btnDelN);
    btnRowN->addStretch();

    nl->addLayout(btnRowN);
    nl->addWidget(m_noticesTable);
    m_tabWidget->addTab(tabNotices, "📢 Manage Notices");

    // 5. FAQS TAB
    auto tabFaqs = new QWidget();
    auto ql = new QVBoxLayout(tabFaqs);
    ql->setContentsMargins(12, 12, 12, 12);
    m_faqsTable = new QTableWidget(tabFaqs);
    m_faqsTable->setColumnCount(3);
    m_faqsTable->setHorizontalHeaderLabels({"Question", "Category", "Views"});
    m_faqsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    auto btnRowQ = new QHBoxLayout();
    auto btnAddQ = new QPushButton("+ Add FAQ", tabFaqs);
    connect(btnAddQ, &QPushButton::clicked, this, &AdminDialog::onAddFaq);
    auto btnDelQ = new QPushButton("🗑️ Delete Selected", tabFaqs);
    btnDelQ->setObjectName("btnSecondary");
    connect(btnDelQ, &QPushButton::clicked, this, &AdminDialog::onDeleteFaq);
    btnRowQ->addWidget(btnAddQ);
    btnRowQ->addWidget(btnDelQ);
    btnRowQ->addStretch();

    ql->addLayout(btnRowQ);
    ql->addWidget(m_faqsTable);
    m_tabWidget->addTab(tabFaqs, "❓ Knowledge Base (FAQs)");

    // 6. SETTINGS TAB
    auto tabSettings = new QWidget();
    auto sl = new QVBoxLayout(tabSettings);
    sl->setContentsMargins(20, 20, 20, 20);
    sl->setSpacing(12);

    m_settingCollegeName = new QLineEdit(tabSettings);
    m_settingAddress = new QLineEdit(tabSettings);
    m_settingPhone = new QLineEdit(tabSettings);
    m_settingEmail = new QLineEdit(tabSettings);

    sl->addWidget(new QLabel("College Name (Branding):", tabSettings));
    sl->addWidget(m_settingCollegeName);
    sl->addWidget(new QLabel("Campus Address:", tabSettings));
    sl->addWidget(m_settingAddress);
    sl->addWidget(new QLabel("Helpline Phone Number:", tabSettings));
    sl->addWidget(m_settingPhone);
    sl->addWidget(new QLabel("Support Email Address:", tabSettings));
    sl->addWidget(m_settingEmail);

    auto btnSaveSettings = new QPushButton("💾 Save College Settings", tabSettings);
    btnSaveSettings->setMinimumHeight(38);
    connect(btnSaveSettings, &QPushButton::clicked, this, &AdminDialog::onSaveSettings);
    sl->addWidget(btnSaveSettings);
    sl->addStretch();

    m_tabWidget->addTab(tabSettings, "⚙️ Campus Settings");

    l->addWidget(m_tabWidget);
}

void AdminDialog::onLoginClicked() {
    QString u = m_usernameEdit->text().trimmed();
    QString p = m_passwordEdit->text().trimmed();

    if (DatabaseManager::instance().verifyAdmin(u, p)) {
        m_lblLoginError->setText("");
        m_stack->setCurrentWidget(m_dashboardWidget);
        refreshAnalytics();
        refreshBuildingsTable();
        refreshFacilitiesTable();
        refreshNoticesTable();
        refreshFaqsTable();

        // Load settings
        auto& db = DatabaseManager::instance();
        m_settingCollegeName->setText(db.getSetting("collegeName"));
        m_settingAddress->setText(db.getSetting("address"));
        m_settingPhone->setText(db.getSetting("contactPhone"));
        m_settingEmail->setText(db.getSetting("contactEmail"));
    } else {
        m_lblLoginError->setText("Invalid admin username or password.");
    }
}

void AdminDialog::onLogoutClicked() {
    m_passwordEdit->clear();
    m_stack->setCurrentWidget(m_loginWidget);
}

void AdminDialog::refreshAnalytics() {
    QMap<QString, int> stats = DatabaseManager::instance().getChatbotAnalytics();
    int totalQ = stats.value("total_queries", 0);
    int helpful = stats.value("helpful_count", 0);
    int unhelpful = stats.value("unhelpful_count", 0);
    int rated = helpful + unhelpful;

    m_statQueries->setText(QString::number(totalQ));
    m_statHelpful->setText(QString::number(helpful));
    if (rated > 0) {
        int pct = static_cast<int>((helpful * 100.0) / rated);
        m_statRatio->setText(QString("%1%").arg(pct));
    } else {
        m_statRatio->setText("100%");
    }
    m_statTotalFacs->setText(QString::number(stats.value("total_facilities", 0)));
}

void AdminDialog::refreshBuildingsTable() {
    QList<Building> list = DatabaseManager::instance().getAllBuildings();
    m_buildingsTable->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const auto& b = list[i];
        m_buildingsTable->setItem(i, 0, new QTableWidgetItem(b.code));
        m_buildingsTable->setItem(i, 1, new QTableWidgetItem(b.name));
        m_buildingsTable->setItem(i, 2, new QTableWidgetItem(QString::number(b.floors)));
        m_buildingsTable->setItem(i, 3, new QTableWidgetItem(QString::number(b.yearBuilt)));
        m_buildingsTable->setItem(i, 4, new QTableWidgetItem(b.wheelchairAccessible ? "Yes" : "No"));
        m_buildingsTable->item(i, 0)->setData(Qt::UserRole, b.id);
    }
}

void AdminDialog::refreshFacilitiesTable() {
    QList<Facility> list = DatabaseManager::instance().getAllFacilities();
    m_facilitiesTable->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const auto& f = list[i];
        m_facilitiesTable->setItem(i, 0, new QTableWidgetItem(f.name));
        m_facilitiesTable->setItem(i, 1, new QTableWidgetItem(f.category));
        m_facilitiesTable->setItem(i, 2, new QTableWidgetItem(f.buildingName));
        m_facilitiesTable->setItem(i, 3, new QTableWidgetItem(f.floor));
        m_facilitiesTable->setItem(i, 4, new QTableWidgetItem(f.timings));
        m_facilitiesTable->item(i, 0)->setData(Qt::UserRole, f.id);
    }
}

void AdminDialog::refreshNoticesTable() {
    QList<Notice> list = DatabaseManager::instance().getAllNotices();
    m_noticesTable->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const auto& n = list[i];
        m_noticesTable->setItem(i, 0, new QTableWidgetItem(n.title));
        m_noticesTable->setItem(i, 1, new QTableWidgetItem(n.category));
        m_noticesTable->setItem(i, 2, new QTableWidgetItem(n.priority));
        m_noticesTable->setItem(i, 3, new QTableWidgetItem(n.author));
        m_noticesTable->item(i, 0)->setData(Qt::UserRole, n.id);
    }
}

void AdminDialog::refreshFaqsTable() {
    QList<Faq> list = DatabaseManager::instance().getAllFaqs();
    m_faqsTable->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const auto& q = list[i];
        m_faqsTable->setItem(i, 0, new QTableWidgetItem(q.question));
        m_faqsTable->setItem(i, 1, new QTableWidgetItem(q.category));
        m_faqsTable->setItem(i, 2, new QTableWidgetItem(QString::number(q.viewCount)));
        m_faqsTable->item(i, 0)->setData(Qt::UserRole, q.id);
    }
}

void AdminDialog::onAddBuilding() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Building", "Building Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    QString code = QInputDialog::getText(this, "Add Building", "Building Code (e.g., BLK-C):", QLineEdit::Normal, "", &ok);
    if (!ok || code.isEmpty()) return;

    Building b;
    b.name = name;
    b.code = code;
    b.description = "New campus facility block.";
    b.floors = 3;
    b.yearBuilt = 2026;
    b.wheelchairAccessible = true;
    b.mapX = 250;
    b.mapY = 60;
    b.mapWidth = 140;
    b.mapHeight = 90;
    b.color = "#4338ca";

    DatabaseManager::instance().addBuilding(b);
    refreshBuildingsTable();
    emit dataModified();
}

void AdminDialog::onDeleteBuilding() {
    int row = m_buildingsTable->currentRow();
    if (row < 0) return;
    QString id = m_buildingsTable->item(row, 0)->data(Qt::UserRole).toString();
    DatabaseManager::instance().deleteBuilding(id);
    refreshBuildingsTable();
    emit dataModified();
}

void AdminDialog::onAddFacility() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Facility", "Facility Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    QString cat = QInputDialog::getText(this, "Add Facility", "Category (Academic, Library, Cafeteria, Sports, Lab, Hostel, Administration, Medical):", QLineEdit::Normal, "Academic", &ok);

    Facility f;
    f.name = name;
    f.category = cat.isEmpty() ? "Academic" : cat;
    f.description = "Newly added campus service.";
    f.floor = "Ground Floor";
    f.timings = "9:00 AM - 5:00 PM";
    f.isOperational = true;

    DatabaseManager::instance().addFacility(f);
    refreshFacilitiesTable();
    emit dataModified();
}

void AdminDialog::onDeleteFacility() {
    int row = m_facilitiesTable->currentRow();
    if (row < 0) return;
    QString id = m_facilitiesTable->item(row, 0)->data(Qt::UserRole).toString();
    DatabaseManager::instance().deleteFacility(id);
    refreshFacilitiesTable();
    emit dataModified();
}

void AdminDialog::onAddNotice() {
    bool ok;
    QString title = QInputDialog::getText(this, "Post Notice", "Notice Title:", QLineEdit::Normal, "", &ok);
    if (!ok || title.isEmpty()) return;
    QString content = QInputDialog::getText(this, "Post Notice", "Notice Content:", QLineEdit::Normal, "", &ok);
    if (!ok || content.isEmpty()) return;

    Notice n;
    n.title = title;
    n.content = content;
    n.category = "General";
    n.priority = "Normal";
    n.author = "College Administration";
    n.publishDate = QDateTime::currentDateTime();
    n.isPublished = true;

    DatabaseManager::instance().addNotice(n);
    refreshNoticesTable();
    emit dataModified();
}

void AdminDialog::onDeleteNotice() {
    int row = m_noticesTable->currentRow();
    if (row < 0) return;
    QString id = m_noticesTable->item(row, 0)->data(Qt::UserRole).toString();
    DatabaseManager::instance().deleteNotice(id);
    refreshNoticesTable();
    emit dataModified();
}

void AdminDialog::onAddFaq() {
    bool ok;
    QString q = QInputDialog::getText(this, "Add FAQ", "Question:", QLineEdit::Normal, "", &ok);
    if (!ok || q.isEmpty()) return;
    QString a = QInputDialog::getText(this, "Add FAQ", "Answer:", QLineEdit::Normal, "", &ok);
    if (!ok || a.isEmpty()) return;

    Faq f;
    f.question = q;
    f.answer = a;
    f.category = "General";
    f.keywords = q.toLower();
    f.viewCount = 0;

    DatabaseManager::instance().addFaq(f);
    refreshFaqsTable();
    emit dataModified();
}

void AdminDialog::onDeleteFaq() {
    int row = m_faqsTable->currentRow();
    if (row < 0) return;
    QString id = m_faqsTable->item(row, 0)->data(Qt::UserRole).toString();
    DatabaseManager::instance().deleteFaq(id);
    refreshFaqsTable();
    emit dataModified();
}

void AdminDialog::onSaveSettings() {
    auto& db = DatabaseManager::instance();
    db.setSetting("collegeName", m_settingCollegeName->text());
    db.setSetting("address", m_settingAddress->text());
    db.setSetting("contactPhone", m_settingPhone->text());
    db.setSetting("contactEmail", m_settingEmail->text());

    QMessageBox::information(this, "Settings Saved", "Campus settings have been updated successfully!");
    emit dataModified();
}
