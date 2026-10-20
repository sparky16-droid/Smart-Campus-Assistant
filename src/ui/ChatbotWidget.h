#ifndef CHATBOTWIDGET_H
#define CHATBOTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include "../nlp/ChatbotEngine.h"

class ChatBubble : public QFrame {
    Q_OBJECT

public:
    ChatBubble(const QString& role, const QString& content, const QString& messageId = "", const ChatReference& ref = ChatReference(), QWidget* parent = nullptr);

signals:
    void locateOnMapRequested(const QString& locationName);
    void questionSuggested(const QString& question);

private:
    QString m_messageId;
    QPushButton* m_btnHelpful;
    QPushButton* m_btnUnhelpful;
    QLabel* m_lblFeedbackStatus;
};

class ChatbotWidget : public QWidget {
    Q_OBJECT

public:
    explicit ChatbotWidget(QWidget* parent = nullptr);
    void sendQuery(const QString& message);

signals:
    void navigateToMap(const QString& placeName);

private slots:
    void onSendClicked();
    void onClearChatClicked();
    void onChipClicked();

private:
    void addMessageBubble(const QString& role, const QString& content, const QString& messageId = "", const ChatReference& ref = ChatReference());
    void updateSuggestedChips(const QStringList& suggestions);

    QVBoxLayout* m_messagesLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_messagesContainer;
    QLineEdit* m_inputEdit;
    QPushButton* m_btnSend;
    QPushButton* m_btnClear;
    QHBoxLayout* m_chipsLayout;
    QWidget* m_chipsContainer;

    QString m_sessionId;
};

#endif // CHATBOTWIDGET_H
