#include "HomeWidget.h"
#include "../database/DatabaseManager.h"

HomeWidget::HomeWidget(QWidget* parent) : QWidget(parent) {
    auto rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto contentWidget = new QWidget(scrollArea);
    auto mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setContentsMargins(24, 20, 24, 24);
    mainLayout->setSpacing(20);

    // 1. HERO BANNER
    auto heroFrame = new QFrame(contentWidget);
    heroFrame->setObjectName("cardFrame");
    heroFrame->setStyleSheet("QFrame#cardFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0f172a, stop:1 #1e3a8a); border-radius: 14px; padding: 24px; border: none; }");

    auto heroLayout = new QVBoxLayout(heroFrame);
    heroLayout->setSpacing(12);

    auto lblBadge = new QLabel("🎓 COLLEGE PROJECT-BASED LEARNING (PBL) PLATFORM", heroFrame);
    lblBadge->setStyleSheet("color: #38bdf8; font-weight: 700; font-size: 11px; letter-spacing: 1px;");

    m_lblCollegeName = new QLabel("Apex Institute of Technology & Management (Demo Campus)", heroFrame);
    m_lblCollegeName->setStyleSheet("color: #ffffff; font-size: 24px; font-weight: 800;");
    m_lblCollegeName->setWordWrap(true);

    m_lblCollegeTagline = new QLabel("A centralized digital assistant providing intelligent campus navigation, instant AI answers, and comprehensive college directories.", heroFrame);
    m_lblCollegeTagline->setStyleSheet("color: #cbd5e1; font-size: 14px; line-height: 1.5;");
    m_lblCollegeTagline->setWordWrap(true);

    // Hero Action Buttons
    auto heroBtnLayout = new QHBoxLayout();
    heroBtnLayout->setSpacing(12);

    auto btnAsk = new QPushButton("🤖 Ask Campus Assistant", heroFrame);
    btnAsk->setObjectName("btnAccent");
    btnAsk->setMinimumHeight(42);
    btnAsk->setStyleSheet("font-size: 14px; font-weight: 700; padding: 10px 20px; border-radius: 8px;");
    connect(btnAsk, &QPushButton::clicked, [this]() { emit navigateToPage(1); });

    auto btnMap = new QPushButton("🗺️ Explore Interactive Map", heroFrame);
    btnMap->setMinimumHeight(42);
    btnMap->setStyleSheet("background-color: #ffffff; color: #0f172a; font-size: 14px; font-weight: 700; padding: 10px 20px; border-radius: 8px; border: none;");
    connect(btnMap, &QPushButton::clicked, [this]() { emit navigateToPage(2); });

    heroBtnLayout->addWidget(btnAsk);
    heroBtnLayout->addWidget(btnMap);
    heroBtnLayout->addStretch();

    heroLayout->addWidget(lblBadge);
    heroLayout->addWidget(m_lblCollegeName);
    heroLayout->addWidget(m_lblCollegeTagline);
    heroLayout->addLayout(heroBtnLayout);

    mainLayout->addWidget(heroFrame);

    // 2. SEARCH BAR
    auto searchCard = new QFrame(contentWidget);
    searchCard->setObjectName("cardFrame");
    auto searchLayout = new QHBoxLayout(searchCard);
    searchLayout->setContentsMargins(16, 12, 16, 12);

    m_searchEdit = new QLineEdit(searchCard);
    m_searchEdit->setPlaceholderText("🔍 Quick search departments, faculty, library timings, cafeterias, hostel rules, or exams...");
    m_searchEdit->setMinimumHeight(40);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &HomeWidget::onSearchTriggered);

    auto btnSearch = new QPushButton("Search Campus", searchCard);
    btnSearch->setMinimumHeight(40);
    connect(btnSearch, &QPushButton::clicked, this, &HomeWidget::onSearchTriggered);

    searchLayout->addWidget(m_searchEdit, 1);
    searchLayout->addWidget(btnSearch);
    mainLayout->addWidget(searchCard);

    // 3. CAMPUS STATISTICS ROW
    auto statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(16);

    auto createStatCard = [](const QString& count, const QString& label, const QString& icon, const QString& color) {
        auto card = new QFrame();
        card->setObjectName("cardFrame");
        auto l = new QVBoxLayout(card);
        l->setContentsMargins(16, 14, 16, 14);
        l->setSpacing(4);

        auto lblIcon = new QLabel(icon);
        lblIcon->setStyleSheet("font-size: 20px;");

        auto lblCount = new QLabel(count);
        lblCount->setStyleSheet(QString("font-size: 22px; font-weight: 800; color: %1;").arg(color));

        auto lblName = new QLabel(label);
        lblName->setStyleSheet("color: #64748b; font-size: 12px; font-weight: 600;");

        l->addWidget(lblIcon);
        l->addWidget(lblCount);
        l->addWidget(lblName);
        return card;
    };

    auto bldCard = createStatCard("8", "Campus Buildings", "🏛️", "#1e40af");
    auto deptCard = createStatCard("6", "Departments", "🎓", "#0d9488");
    auto facCard = createStatCard("8", "Faculty Members", "👨‍🏫", "#d97706");
    auto servCard = createStatCard("13", "Facilities & Services", "🏢", "#059669");

    statsLayout->addWidget(bldCard);
    statsLayout->addWidget(deptCard);
    statsLayout->addWidget(facCard);
    statsLayout->addWidget(servCard);
    mainLayout->addLayout(statsLayout);

    // 4. QUICK ACCESS GRID
    auto lblSection = new QLabel("Campus Quick Access", contentWidget);
    lblSection->setStyleSheet("font-size: 17px; font-weight: 700; color: #0f172a; margin-top: 8px;");
    mainLayout->addWidget(lblSection);

    auto quickGrid = new QGridLayout();
    quickGrid->setSpacing(12);

    struct QItem {
        QString title, desc, icon;
        int targetPage;
        QString question;
    };

    QList<QItem> qItems = {
        {"Academic Departments", "B.Tech, M.Tech, and MBA programs", "🎓", 3, ""},
        {"Faculty Directory", "Professor contacts, cabins & office hours", "👨‍🏫", 4, ""},
        {"Campus Map", "Interactive 2D schematic & coordinates", "🗺️", 2, ""},
        {"Facilities & Services", "Library, Cafeteria, Sports & Clinic", "🏢", 5, ""},
        {"Central Library", "Timings, reading rooms, books catalog", "📚", 1, "Where is the library and what are its timings?"},
        {"Campus Cafeteria", "Menus, meal timings & food court", "🍽️", 1, "Where is the college cafeteria and what are the food options?"},
        {"Official Notices", "Examinations, hackathons & circulars", "📢", 6, ""},
        {"Frequently Asked Questions", "Common questions answered instantly", "❓", 7, ""}
    };

    for (int i = 0; i < qItems.size(); ++i) {
        const auto& it = qItems[i];
        auto card = new QFrame(contentWidget);
        card->setObjectName("cardFrame");
        card->setCursor(Qt::PointingHandCursor);

        auto cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 14, 16, 14);
        cl->setSpacing(4);

        auto h = new QHBoxLayout();
        auto lblI = new QLabel(it.icon, card);
        lblI->setStyleSheet("font-size: 22px;");
        auto lblT = new QLabel(it.title, card);
        lblT->setStyleSheet("font-weight: 700; font-size: 14px; color: #0f172a;");
        h->addWidget(lblI);
        h->addWidget(lblT, 1);

        auto lblD = new QLabel(it.desc, card);
        lblD->setStyleSheet("color: #64748b; font-size: 12px;");

        cl->addLayout(h);
        cl->addWidget(lblD);

        // Click handler
        auto btnOverlay = new QPushButton(card);
        btnOverlay->setStyleSheet("background: transparent; border: none;");
        btnOverlay->setGeometry(0, 0, 300, 100);

        int page = it.targetPage;
        QString qst = it.question;
        connect(btnOverlay, &QPushButton::clicked, [this, page, qst]() {
            if (!qst.isEmpty()) {
                emit askAssistantQuestion(qst);
            } else {
                emit navigateToPage(page);
            }
        });

        quickGrid->addWidget(card, i / 4, i % 4);
    }
    mainLayout->addLayout(quickGrid);

    // 5. SPLIT SECTION: NOTICES TICKER & POPULAR QUESTIONS
    auto splitLayout = new QHBoxLayout();
    splitLayout->setSpacing(16);

    // Left: Latest Notices
    auto noticesCard = new QFrame(contentWidget);
    noticesCard->setObjectName("cardFrame");
    auto noticesLayout = new QVBoxLayout(noticesCard);
    noticesLayout->setContentsMargins(18, 16, 18, 16);

    auto lblNoticesHeader = new QLabel("📢 Latest Campus Announcements", noticesCard);
    lblNoticesHeader->setStyleSheet("font-size: 15px; font-weight: 700; color: #0f172a;");
    noticesLayout->addWidget(lblNoticesHeader);

    m_noticesFeedContainer = new QWidget(noticesCard);
    m_noticesFeedLayout = new QVBoxLayout(m_noticesFeedContainer);
    m_noticesFeedLayout->setContentsMargins(0, 8, 0, 0);
    m_noticesFeedLayout->setSpacing(8);
    noticesLayout->addWidget(m_noticesFeedContainer);

    auto btnAllNotices = new QPushButton("View All Notices →", noticesCard);
    btnAllNotices->setObjectName("btnSecondary");
    connect(btnAllNotices, &QPushButton::clicked, [this]() { emit navigateToPage(6); });
    noticesLayout->addWidget(btnAllNotices);

    // Right: Popular Questions
    auto popularCard = new QFrame(contentWidget);
    popularCard->setObjectName("cardFrame");
    auto popLayout = new QVBoxLayout(popularCard);
    popLayout->setContentsMargins(18, 16, 18, 16);

    auto lblPopHeader = new QLabel("💡 Popular Questions Asked", popularCard);
    lblPopHeader->setStyleSheet("font-size: 15px; font-weight: 700; color: #0f172a;");
    popLayout->addWidget(lblPopHeader);

    QStringList popularQuestions = {
        "Where is the Central Library and what are its timings?",
        "Where is the cafeteria located?",
        "How can I reach the Computer Science department?",
        "What are the campus clinic and doctor hours?",
        "Where are the ATMs located?"
    };

    for (const auto& pq : popularQuestions) {
        auto btnQ = new QPushButton("💬 " + pq, popularCard);
        btnQ->setStyleSheet("text-align: left; background: #f8fafc; border: 1px solid #e2e8f0; color: #1e293b; padding: 8px 12px; border-radius: 6px; font-size: 12px; font-weight: 500;");
        connect(btnQ, &QPushButton::clicked, [this, pq]() {
            emit askAssistantQuestion(pq);
        });
        popLayout->addWidget(btnQ);
    }
    popLayout->addStretch();

    splitLayout->addWidget(noticesCard, 1);
    splitLayout->addWidget(popularCard, 1);
    mainLayout->addLayout(splitLayout);

    // 6. DISCLAIMER BANNER
    auto noticeBanner = new QLabel("⚠️ Demonstration Notice: This application is running on sample college data for Project-Based Learning (PBL). All faculty, timings, and campus records are illustrative.", contentWidget);
    noticeBanner->setStyleSheet("background: #fef3c7; color: #92400e; padding: 10px 16px; border-radius: 8px; font-size: 12px; font-weight: 500; border: 1px solid #fde68a;");
    noticeBanner->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(noticeBanner);

    scrollArea->setWidget(contentWidget);
    rootLayout->addWidget(scrollArea);

    refreshData();
}

void HomeWidget::refreshData() {
    // Clear and reload recent notices
    QLayoutItem* item;
    while ((item = m_noticesFeedLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    QList<Notice> notices = DatabaseManager::instance().getAllNotices();
    int count = 0;
    for (const auto& n : notices) {
        if (count >= 3) break;
        auto itemFrame = new QFrame(m_noticesFeedContainer);
        itemFrame->setStyleSheet("background: #f8fafc; border: 1px solid #e2e8f0; border-radius: 6px; padding: 8px;");

        auto il = new QVBoxLayout(itemFrame);
        il->setContentsMargins(8, 6, 8, 6);
        il->setSpacing(2);

        auto hl = new QHBoxLayout();
        auto lblTitle = new QLabel(n.title, itemFrame);
        lblTitle->setStyleSheet("font-weight: 700; color: #1e40af; font-size: 12px;");

        QString badgeColor = (n.priority == "Urgent") ? "#ef4444" : ((n.priority == "High") ? "#f59e0b" : "#3b82f6");
        auto lblPriority = new QLabel(n.priority, itemFrame);
        lblPriority->setStyleSheet(QString("background: %1; color: white; padding: 2px 6px; border-radius: 4px; font-size: 10px; font-weight: 700;").arg(badgeColor));

        hl->addWidget(lblTitle, 1);
        hl->addWidget(lblPriority);

        auto lblSnippet = new QLabel(n.content.left(100) + "...", itemFrame);
        lblSnippet->setStyleSheet("color: #64748b; font-size: 11px;");

        il->addLayout(hl);
        il->addWidget(lblSnippet);
        m_noticesFeedLayout->addWidget(itemFrame);
        count++;
    }
}

void HomeWidget::onSearchTriggered() {
    QString q = m_searchEdit->text().trimmed();
    if (!q.isEmpty()) {
        emit askAssistantQuestion(q);
    }
}
