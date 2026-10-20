#include "NoticesFaqWidgets.h"
#include "../database/DatabaseManager.h"
#include <QFrame>

// ==========================================
// 1. NOTICES WIDGET
// ==========================================
NoticesWidget::NoticesWidget(QWidget* parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(14);

    auto lblTitle = new QLabel("Campus Notices & Announcements", this);
    lblTitle->setStyleSheet("font-size: 20px; font-weight: 700; color: #0f172a;");
    auto lblSub = new QLabel("Official circulars, exam notifications, placement alerts, and campus event schedules", this);
    lblSub->setStyleSheet("color: #64748b; font-size: 13px;");

    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblSub);

    auto filterBar = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("🔍 Search notices by title or content...");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &NoticesWidget::applyFilter);

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItems({"All Categories", "Exam", "Academic", "Event", "Placement", "General"});
    connect(m_categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NoticesWidget::applyFilter);

    filterBar->addWidget(m_searchEdit, 2);
    filterBar->addWidget(m_categoryCombo, 1);
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

void NoticesWidget::refreshData() {
    m_noticesList = DatabaseManager::instance().getAllNotices();
    applyFilter();
}

void NoticesWidget::applyFilter() {
    while (m_cardsLayout->count() > 1) {
        QLayoutItem* item = m_cardsLayout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }

    QString search = m_searchEdit->text().trimmed();
    QString cat = m_categoryCombo->currentText();

    for (const auto& n : m_noticesList) {
        if (cat != "All Categories" && n.category != cat) continue;
        if (!search.isEmpty()) {
            bool match = n.title.contains(search, Qt::CaseInsensitive) ||
                         n.content.contains(search, Qt::CaseInsensitive) ||
                         n.author.contains(search, Qt::CaseInsensitive);
            if (!match) continue;
        }

        auto card = new QFrame(m_cardsContainer);
        card->setObjectName("cardFrame");
        auto cl = new QVBoxLayout(card);
        cl->setContentsMargins(18, 16, 18, 16);
        cl->setSpacing(8);

        auto hl = new QHBoxLayout();
        auto lblTitle = new QLabel(n.title, card);
        lblTitle->setStyleSheet("font-size: 15px; font-weight: 700; color: #1e40af;");

        QString badgeColor = (n.priority == "Urgent") ? "#ef4444" : ((n.priority == "High") ? "#f59e0b" : "#3b82f6");
        auto lblPriority = new QLabel(n.priority, card);
        lblPriority->setStyleSheet(QString("background: %1; color: white; padding: 3px 8px; border-radius: 4px; font-size: 10px; font-weight: 700;").arg(badgeColor));

        hl->addWidget(lblTitle, 1);
        hl->addWidget(lblPriority);
        cl->addLayout(hl);

        auto lblMeta = new QLabel(QString("📅 <b>Published:</b> %1  |  🏛️ <b>Issued by:</b> %2  |  🏷️ <b>Category:</b> %3")
                                  .arg(n.publishDate.toString("MMMM dd, yyyy"))
                                  .arg(n.author)
                                  .arg(n.category), card);
        lblMeta->setStyleSheet("color: #64748b; font-size: 12px;");

        auto lblContent = new QLabel(n.content, card);
        lblContent->setWordWrap(true);
        lblContent->setStyleSheet("color: #334155; font-size: 13px; line-height: 1.5;");

        cl->addWidget(lblMeta);
        cl->addWidget(lblContent);

        m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
    }
}

// ==========================================
// 2. FAQS WIDGET
// ==========================================
FaqWidget::FaqWidget(QWidget* parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(14);

    auto lblTitle = new QLabel("Frequently Asked Questions (FAQs)", this);
    lblTitle->setStyleSheet("font-size: 20px; font-weight: 700; color: #0f172a;");
    auto lblSub = new QLabel("Instant answers to common student, parent, visitor, and faculty campus inquiries", this);
    lblSub->setStyleSheet("color: #64748b; font-size: 13px;");

    mainLayout->addWidget(lblTitle);
    mainLayout->addWidget(lblSub);

    auto filterBar = new QHBoxLayout();
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("🔍 Search FAQs by question or keyword...");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &FaqWidget::applyFilter);

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItems({"All Categories", "Facilities", "Academics", "Administration", "Hostel", "Transport"});
    connect(m_categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FaqWidget::applyFilter);

    filterBar->addWidget(m_searchEdit, 2);
    filterBar->addWidget(m_categoryCombo, 1);
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

void FaqWidget::refreshData() {
    m_faqsList = DatabaseManager::instance().getAllFaqs();
    applyFilter();
}

void FaqWidget::applyFilter() {
    while (m_cardsLayout->count() > 1) {
        QLayoutItem* item = m_cardsLayout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }

    QString search = m_searchEdit->text().trimmed();
    QString cat = m_categoryCombo->currentText();

    for (const auto& f : m_faqsList) {
        if (cat != "All Categories" && f.category != cat) continue;
        if (!search.isEmpty()) {
            bool match = f.question.contains(search, Qt::CaseInsensitive) ||
                         f.answer.contains(search, Qt::CaseInsensitive) ||
                         f.keywords.contains(search, Qt::CaseInsensitive);
            if (!match) continue;
        }

        auto card = new QFrame(m_cardsContainer);
        card->setObjectName("cardFrame");
        auto cl = new QVBoxLayout(card);
        cl->setContentsMargins(18, 16, 18, 16);
        cl->setSpacing(8);

        auto hl = new QHBoxLayout();
        auto lblQ = new QLabel("❓ " + f.question, card);
        lblQ->setStyleSheet("font-size: 15px; font-weight: 700; color: #0f172a;");

        auto lblCat = new QLabel(f.category, card);
        lblCat->setStyleSheet("background: #f1f5f9; color: #475569; padding: 3px 8px; border-radius: 4px; font-size: 11px; font-weight: 600;");

        auto btnAsk = new QPushButton("💬 Ask Assistant", card);
        btnAsk->setObjectName("btnSecondary");
        btnAsk->setStyleSheet("font-size: 11px; padding: 4px 10px;");
        QString qst = f.question;
        connect(btnAsk, &QPushButton::clicked, [this, qst]() {
            emit askAssistantQuestion(qst);
        });

        hl->addWidget(lblQ, 1);
        hl->addWidget(lblCat);
        hl->addWidget(btnAsk);
        cl->addLayout(hl);

        auto lblAns = new QLabel(f.answer, card);
        lblAns->setWordWrap(true);
        lblAns->setStyleSheet("color: #334155; font-size: 13px; line-height: 1.5; background: #f8fafc; padding: 10px; border-radius: 6px;");

        auto lblViews = new QLabel(QString("👁️ Viewed %1 times").arg(f.viewCount), card);
        lblViews->setStyleSheet("color: #94a3b8; font-size: 11px;");

        cl->addWidget(lblAns);
        cl->addWidget(lblViews);

        m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
    }
}

// ==========================================
// 3. ABOUT & CONTACT WIDGET
// ==========================================
AboutWidget::AboutWidget(QWidget* parent) : QWidget(parent) {
    auto rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto content = new QWidget(scrollArea);
    auto l = new QVBoxLayout(content);
    l->setContentsMargins(24, 20, 24, 24);
    l->setSpacing(20);

    auto lblTitle = new QLabel("About Smart Campus Assistant & Contact Directory", content);
    lblTitle->setStyleSheet("font-size: 22px; font-weight: 800; color: #0f172a;");
    l->addWidget(lblTitle);

    // Campus Contact Information Card
    auto contactCard = new QFrame(content);
    contactCard->setObjectName("cardFrame");
    auto cl = new QVBoxLayout(contactCard);
    cl->setContentsMargins(20, 18, 20, 18);
    cl->setSpacing(10);

    auto lblContactHead = new QLabel("🏢 Central Campus Contact Details", contactCard);
    lblContactHead->setStyleSheet("font-size: 16px; font-weight: 700; color: #1e40af;");

    auto lblAddress = new QLabel("📍 <b>Address:</b> Knowledge City, Tech Boulevard, Sector 42, Metro Campus - 560001", contactCard);
    auto lblPhone = new QLabel("📞 <b>Helpline:</b> +91 (080) 4123-4567 / 89   |   🚨 <b>Emergency Ambulance:</b> +91 (080) 4123-4599", contactCard);
    auto lblEmail = new QLabel("✉️ <b>General Email:</b> assistant@smartcampus.edu   |   <b>Admissions:</b> admissions@smartcampus.edu", contactCard);
    auto lblHours = new QLabel("⏰ <b>Administrative Office Hours:</b> Monday to Saturday, 8:30 AM to 5:30 PM", contactCard);

    cl->addWidget(lblContactHead);
    cl->addWidget(lblAddress);
    cl->addWidget(lblPhone);
    cl->addWidget(lblEmail);
    cl->addWidget(lblHours);
    l->addWidget(contactCard);

    // PBL Project & Architecture Card
    auto pblCard = new QFrame(content);
    pblCard->setObjectName("cardFrame");
    auto pl = new QVBoxLayout(pblCard);
    pl->setContentsMargins(20, 18, 20, 18);
    pl->setSpacing(10);

    auto lblPblHead = new QLabel("🎓 College Project-Based Learning (PBL) Architecture", pblCard);
    lblPblHead->setStyleSheet("font-size: 16px; font-weight: 700; color: #059669;");

    QString pblText = "<b>Project Name:</b> Smart Campus Assistant<br/>"
                      "<b>Technology Stack:</b> C++ (C++17), Qt 6.12 (QtWidgets, QtSql, QtNetwork), SQLite database, Embedded Web Server<br/>"
                      "<b>Natural Language Engine:</b> In-memory synonym dictionary expansion, token normalization, weighted relevance scoring, conversational context memory, and anti-hallucination ground truth verification.<br/>"
                      "<b>Map System:</b> Custom 2D schematic graphics engine with interactive building hitboxes, floor directories, zoom, pan, and coordinate markers.<br/>"
                      "<b>Admin Control:</b> In-app administrator panel with password verification, database metrics, and full CRUD for campus data.";

    auto lblPbl = new QLabel(pblText, pblCard);
    lblPbl->setWordWrap(true);
    lblPbl->setStyleSheet("color: #334155; font-size: 13px; line-height: 1.6;");

    pl->addWidget(lblPblHead);
    pl->addWidget(lblPbl);
    l->addWidget(pblCard);

    // Presentation Guide Card
    auto guideCard = new QFrame(content);
    guideCard->setObjectName("cardFrame");
    auto gl = new QVBoxLayout(guideCard);
    gl->setContentsMargins(20, 18, 20, 18);
    gl->setSpacing(8);

    auto lblGuideHead = new QLabel("💡 Guide for Presenting this Project to College Examiners", guideCard);
    lblGuideHead->setStyleSheet("font-size: 16px; font-weight: 700; color: #d97706;");

    QString guideText = "1. <b>Demonstrate the Chatbot:</b> Ask 'Where is the library?', then ask a follow-up pronoun question 'What are its timings?' to highlight conversational context resolution.<br/>"
                        "2. <b>Demonstrate the Interactive Map:</b> Click on 'Main Academic Block' or 'Science Tower' to show building details and instant 'Ask Assistant' linking.<br/>"
                        "3. <b>Demonstrate Search:</b> Type any faculty name or lab to showcase instant global retrieval across multiple database models.<br/>"
                        "4. <b>Demonstrate Admin Portal:</b> Open the Admin Dashboard (username: <code>admin</code>, password: <code>adminpassword123</code>) to show live queries and add a new announcement.<br/>"
                        "5. <b>Explain Anti-Hallucination:</b> Emphasize that unlike unconstrained LLMs, this assistant strictly verifies information against the college database before answering.";

    auto lblGuide = new QLabel(guideText, guideCard);
    lblGuide->setWordWrap(true);
    lblGuide->setStyleSheet("color: #334155; font-size: 13px; line-height: 1.6;");

    gl->addWidget(lblGuideHead);
    gl->addWidget(lblGuide);
    l->addWidget(guideCard);

    scrollArea->setWidget(content);
    rootLayout->addWidget(scrollArea);
}
