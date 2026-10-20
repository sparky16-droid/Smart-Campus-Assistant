#include "ChatbotWidget.h"
#include "../database/DatabaseManager.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QScrollBar>
#include <QTimer>
#include <QRegularExpression>

ChatBubble::ChatBubble(const QString& role, const QString& content, const QString& messageId, const ChatReference& ref, QWidget* parent)
    : QFrame(parent), m_messageId(messageId) {
    setObjectName("chatBubble");

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 12, 14, 12);
    mainLayout->setSpacing(8);

    bool isUser = (role == "user");

    // Header info (Sender and Time)
    auto headerLayout = new QHBoxLayout();
    auto lblSender = new QLabel(isUser ? "👤 You" : "🤖 Smart Campus Assistant", this);
    lblSender->setStyleSheet(isUser ? "font-weight: 700; color: #bfdbfe; font-size: 11px;" : "font-weight: 700; color: #1e40af; font-size: 11px;");

    headerLayout->addWidget(lblSender);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // Message Body
    // Convert basic markdown formatting to clean HTML
    QString htmlContent = content;
    htmlContent.replace("\n", "<br/>");
    htmlContent.replace("### ", "<b><font color='#1e3a8a'>");
    htmlContent.replace(QRegularExpression("\\*\\*(.*?)\\*\\*"), "<b>\\1</b>");
    htmlContent.replace(QRegularExpression("\\*(.*?)\\*"), "<i>\\1</i>");

    auto lblContent = new QLabel(htmlContent, this);
    lblContent->setWordWrap(true);
    lblContent->setTextInteractionFlags(Qt::TextSelectableByMouse);
    lblContent->setStyleSheet(isUser ? "color: #ffffff; font-size: 13px;" : "color: #1e293b; font-size: 13px;");
    mainLayout->addWidget(lblContent);

    // If Assistant bubble, add location button, copy button, and feedback controls
    if (!isUser) {
        auto actionsLayout = new QHBoxLayout();
        actionsLayout->setContentsMargins(0, 4, 0, 0);

        // Copy button
        auto btnCopy = new QPushButton("📋 Copy", this);
        btnCopy->setObjectName("btnSecondary");
        btnCopy->setStyleSheet("font-size: 11px; padding: 3px 8px;");
        connect(btnCopy, &QPushButton::clicked, [content]() {
            QGuiApplication::clipboard()->setText(content);
        });
        actionsLayout->addWidget(btnCopy);

        // Show on Map action if referenced
        if (!ref.name.isEmpty()) {
            auto btnMap = new QPushButton("🗺️ Show on Map", this);
            btnMap->setObjectName("btnAccent");
            btnMap->setStyleSheet("font-size: 11px; padding: 3px 8px;");
            QString place = ref.buildingName.isEmpty() ? ref.name : ref.buildingName;
            connect(btnMap, &QPushButton::clicked, [this, place]() {
                emit locateOnMapRequested(place);
            });
            actionsLayout->addWidget(btnMap);
        }

        actionsLayout->addStretch();

        // Feedback buttons
        if (!messageId.isEmpty()) {
            auto lblWasHelpful = new QLabel("Helpful?", this);
            lblWasHelpful->setStyleSheet("color: #64748b; font-size: 11px;");
            actionsLayout->addWidget(lblWasHelpful);

            m_btnHelpful = new QPushButton("👍", this);
            m_btnHelpful->setObjectName("btnSecondary");
            m_btnHelpful->setFixedSize(26, 26);
            m_btnHelpful->setToolTip("Mark as helpful");

            m_btnUnhelpful = new QPushButton("👎", this);
            m_btnUnhelpful->setObjectName("btnSecondary");
            m_btnUnhelpful->setFixedSize(26, 26);
            m_btnUnhelpful->setToolTip("Mark as not helpful");

            m_lblFeedbackStatus = new QLabel("", this);
            m_lblFeedbackStatus->setStyleSheet("color: #059669; font-size: 11px; font-weight: 600;");

            connect(m_btnHelpful, &QPushButton::clicked, [this]() {
                DatabaseManager::instance().recordFeedback(m_messageId, true);
                m_btnHelpful->setEnabled(false);
                m_btnUnhelpful->setEnabled(false);
                m_lblFeedbackStatus->setText("✓ Thanks!");
            });

            connect(m_btnUnhelpful, &QPushButton::clicked, [this]() {
                DatabaseManager::instance().recordFeedback(m_messageId, false);
                m_btnHelpful->setEnabled(false);
                m_btnUnhelpful->setEnabled(false);
                m_lblFeedbackStatus->setText("✓ Noted");
            });

            actionsLayout->addWidget(m_btnHelpful);
            actionsLayout->addWidget(m_btnUnhelpful);
            actionsLayout->addWidget(m_lblFeedbackStatus);
        }

        mainLayout->addLayout(actionsLayout);
    }

    // Styling differences
    if (isUser) {
        setStyleSheet("QFrame#chatBubble { background-color: #1e40af; border-radius: 12px; }");
    } else {
        setStyleSheet("QFrame#chatBubble { background-color: #ffffff; border: 1px solid #e2e8f0; border-radius: 12px; }");
    }
}

// CHATBOT WIDGET
ChatbotWidget::ChatbotWidget(QWidget* parent) : QWidget(parent) {
    m_sessionId = "session-" + QString::number(QDateTime::currentMSecsSinceEpoch());

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 16, 20, 16);
    mainLayout->setSpacing(12);

    // Top Header & Controls
    auto headerLayout = new QHBoxLayout();
    auto titleBox = new QVBoxLayout();

    auto lblTitle = new QLabel("Campus AI Assistant", this);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: 700; color: #0f172a;");

    auto lblStatus = new QLabel("🟢 Grounded Campus Knowledge Base • Anti-Hallucination Enabled • Demo Mode", this);
    lblStatus->setStyleSheet("color: #059669; font-size: 12px; font-weight: 500;");

    titleBox->addWidget(lblTitle);
    titleBox->addWidget(lblStatus);
    headerLayout->addLayout(titleBox);
    headerLayout->addStretch();

    m_btnClear = new QPushButton("🗑️ Clear History", this);
    m_btnClear->setObjectName("btnSecondary");
    connect(m_btnClear, &QPushButton::clicked, this, &ChatbotWidget::onClearChatClicked);
    headerLayout->addWidget(m_btnClear);

    mainLayout->addLayout(headerLayout);

    // Chat Messages Scroll Area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setStyleSheet("background-color: #f8fafc;");

    m_messagesContainer = new QWidget();
    m_messagesLayout = new QVBoxLayout(m_messagesContainer);
    m_messagesLayout->setContentsMargins(10, 10, 10, 10);
    m_messagesLayout->setSpacing(12);
    m_messagesLayout->addStretch();

    m_scrollArea->setWidget(m_messagesContainer);
    mainLayout->addWidget(m_scrollArea, 1);

    // Suggested Questions Chips Container
    m_chipsContainer = new QWidget(this);
    m_chipsLayout = new QHBoxLayout(m_chipsContainer);
    m_chipsLayout->setContentsMargins(4, 0, 4, 0);
    m_chipsLayout->setSpacing(8);
    m_chipsLayout->addStretch();

    mainLayout->addWidget(m_chipsContainer);

    // Bottom Input Box
    auto inputLayout = new QHBoxLayout();
    m_inputEdit = new QLineEdit(this);
    m_inputEdit->setPlaceholderText("Ask a campus question (e.g. 'Where is the library?', 'Cafeteria timings', 'CSE HOD', 'Medical clinic')...");
    m_inputEdit->setMinimumHeight(44);
    connect(m_inputEdit, &QLineEdit::returnPressed, this, &ChatbotWidget::onSendClicked);

    m_btnSend = new QPushButton("Send 🚀", this);
    m_btnSend->setMinimumHeight(44);
    m_btnSend->setFixedWidth(100);
    connect(m_btnSend, &QPushButton::clicked, this, &ChatbotWidget::onSendClicked);

    inputLayout->addWidget(m_inputEdit, 1);
    inputLayout->addWidget(m_btnSend);

    mainLayout->addLayout(inputLayout);

    // Initial greeting bubble
    addMessageBubble("assistant",
                     "Hello! 👋 I am your **Smart Campus Assistant**.\n\n"
                     "You can ask me questions about campus buildings, academic departments, faculty office hours, library books & timings, cafeteria menus, and exam schedules.\n\n"
                     "How can I help you today?");

    updateSuggestedChips({
        "Where is the library?",
        "Where is the cafeteria?",
        "How can I reach the Computer Science department?",
        "What are the clinic timings?",
        "Where is the administration office?"
    });
}

void ChatbotWidget::addMessageBubble(const QString& role, const QString& content, const QString& messageId, const ChatReference& ref) {
    auto bubble = new ChatBubble(role, content, messageId, ref, m_messagesContainer);
    connect(bubble, &ChatBubble::locateOnMapRequested, this, &ChatbotWidget::navigateToMap);

    // Insert before the trailing stretch
    int index = m_messagesLayout->count() - 1;
    m_messagesLayout->insertWidget(index, bubble);

    // Auto scroll down
    QTimer::singleShot(50, [this]() {
        m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->maximum());
    });
}

void ChatbotWidget::updateSuggestedChips(const QStringList& suggestions) {
    // Clear old chips
    QLayoutItem* item;
    while ((item = m_chipsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    auto lblQuick = new QLabel("💡 Quick:", m_chipsContainer);
    lblQuick->setStyleSheet("color: #64748b; font-size: 11px; font-weight: 600;");
    m_chipsLayout->addWidget(lblQuick);

    for (const auto& q : suggestions) {
        auto chip = new QPushButton(q, m_chipsContainer);
        chip->setObjectName("chipBtn");
        connect(chip, &QPushButton::clicked, this, &ChatbotWidget::onChipClicked);
        m_chipsLayout->addWidget(chip);
    }
    m_chipsLayout->addStretch();
}

void ChatbotWidget::onChipClicked() {
    auto chip = qobject_cast<QPushButton*>(sender());
    if (chip) {
        sendQuery(chip->text());
    }
}

void ChatbotWidget::onSendClicked() {
    QString q = m_inputEdit->text().trimmed();
    if (q.isEmpty()) return;
    m_inputEdit->clear();
    sendQuery(q);
}

void ChatbotWidget::sendQuery(const QString& query) {
    // 1. Add User Bubble
    addMessageBubble("user", query);

    // 2. Query ChatbotEngine
    ChatbotAnswer ans = ChatbotEngine::instance().processQuery(query, m_sessionId);

    // 3. Add Assistant Bubble
    addMessageBubble("assistant", ans.answerText, ans.messageId, ans.reference);

    // 4. Update Quick Suggested Questions
    if (!ans.suggestedQuestions.isEmpty()) {
        updateSuggestedChips(ans.suggestedQuestions);
    }
}

void ChatbotWidget::onClearChatClicked() {
    // Delete existing bubbles except the trailing stretch
    while (m_messagesLayout->count() > 1) {
        QLayoutItem* item = m_messagesLayout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }

    ChatbotEngine::instance().clearSession(m_sessionId);

    addMessageBubble("assistant", "Chat history cleared. How can I assist you now?");
    updateSuggestedChips({
        "Where is the library?",
        "Where is the cafeteria?",
        "How can I reach the Computer Science department?"
    });
}
