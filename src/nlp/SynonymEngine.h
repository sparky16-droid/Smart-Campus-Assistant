#ifndef SYNONYMENGINE_H
#define SYNONYMENGINE_H

#include <QString>
#include <QStringList>
#include <QSet>
#include <QMap>

class SynonymEngine {
public:
    static SynonymEngine& instance();

    void loadSynonyms();
    QStringList tokenize(const QString& text) const;
    QSet<QString> expandTokens(const QStringList& tokens);
    bool isStopWord(const QString& word) const;

private:
    SynonymEngine();
    QSet<QString> m_stopWords;
    QMap<QString, QStringList> m_synonymMap;
};

#endif // SYNONYMENGINE_H
