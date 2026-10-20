#ifndef CHATBOTENGINE_H
#define CHATBOTENGINE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include "../database/Models.h"

struct ChatbotAnswer {
    QString answerText;
    double confidence;
    QString source;
    ChatReference reference;
    QStringList suggestedQuestions;
    QString messageId;
};

class ChatbotEngine {
public:
    static ChatbotEngine& instance();

    ChatbotAnswer processQuery(const QString& rawMessage, const QString& sessionId = "default-session");
    void clearSession(const QString& sessionId);

private:
    ChatbotEngine();
    bool isGreeting(const QString& text) const;
    QString resolveContext(const QString& query, const QString& sessionId);
    int calculateScore(const QString& targetText, const QSet<QString>& expandedTokens, const QString& rawPhrase) const;

    QMap<QString, ChatReference> m_sessionContext;
    QSet<QString> m_greetings;
};

#endif // CHATBOTENGINE_H
