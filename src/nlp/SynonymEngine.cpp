#include "SynonymEngine.h"
#include "../database/DatabaseManager.h"
#include <QRegularExpression>

SynonymEngine& SynonymEngine::instance() {
    static SynonymEngine instance;
    return instance;
}

SynonymEngine::SynonymEngine() {
    // Standard conversational stop words
    QStringList stops = {
        "a", "an", "the", "is", "are", "was", "were", "be", "been", "being",
        "in", "on", "at", "to", "for", "with", "about", "against", "between",
        "into", "through", "during", "before", "after", "above", "below",
        "from", "up", "down", "out", "off", "over", "under", "again",
        "further", "then", "once", "here", "there", "when", "why", "how",
        "all", "any", "both", "each", "few", "more", "most", "other", "some",
        "such", "no", "nor", "not", "only", "own", "same", "so", "than",
        "too", "very", "can", "will", "just", "should", "now", "what",
        "where", "who", "which", "tell", "me", "please", "i", "want", "know",
        "give", "detail", "details", "show", "find", "reach", "get", "located",
        "situated", "college", "campus"
    };
    for (const auto& s : stops) {
        m_stopWords.insert(s);
    }

    loadSynonyms();
}

void SynonymEngine::loadSynonyms() {
    m_synonymMap.clear();
    QList<Synonym> list = DatabaseManager::instance().getAllSynonyms();

    for (const auto& syn : list) {
        QString primary = syn.primaryWord.toLower().trimmed();
        QStringList fullGroup = syn.synonyms;
        fullGroup.append(primary);

        m_synonymMap.insert(primary, fullGroup);
        for (const auto& alt : syn.synonyms) {
            QString altClean = alt.toLower().trimmed();
            if (!m_synonymMap.contains(altClean)) {
                m_synonymMap.insert(altClean, fullGroup);
            }
        }
    }
}

bool SynonymEngine::isStopWord(const QString& word) const {
    return m_stopWords.contains(word.toLower().trimmed());
}

QStringList SynonymEngine::tokenize(const QString& text) const {
    QString cleaned = text.toLower();
    cleaned.replace(QRegularExpression("[^a-zA-Z0-9\\s]"), " ");
    QStringList rawWords = cleaned.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    QStringList filtered;
    for (const auto& w : rawWords) {
        QString trimmed = w.trimmed();
        if (trimmed.length() > 1 && !isStopWord(trimmed)) {
            filtered.append(trimmed);
        }
    }
    return filtered;
}

QSet<QString> SynonymEngine::expandTokens(const QStringList& tokens) {
    QSet<QString> expanded;
    for (const auto& t : tokens) {
        QString lower = t.toLower().trimmed();
        expanded.insert(lower);

        if (m_synonymMap.contains(lower)) {
            const auto& synList = m_synonymMap.value(lower);
            for (const auto& s : synList) {
                expanded.insert(s.toLower().trimmed());
            }
        }
    }
    return expanded;
}
