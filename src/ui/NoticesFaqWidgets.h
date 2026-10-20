#ifndef NOTICESFAQWIDGETS_H
#define NOTICESFAQWIDGETS_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "../database/Models.h"

// NOTICES WIDGET
class NoticesWidget : public QWidget {
    Q_OBJECT

public:
    explicit NoticesWidget(QWidget* parent = nullptr);
    void refreshData();

private slots:
    void applyFilter();

private:
    QLineEdit* m_searchEdit;
    QComboBox* m_categoryCombo;
    QVBoxLayout* m_cardsLayout;
    QWidget* m_cardsContainer;
    QList<Notice> m_noticesList;
};

// FAQS WIDGET
class FaqWidget : public QWidget {
    Q_OBJECT

public:
    explicit FaqWidget(QWidget* parent = nullptr);
    void refreshData();

signals:
    void askAssistantQuestion(const QString& question);

private slots:
    void applyFilter();

private:
    QLineEdit* m_searchEdit;
    QComboBox* m_categoryCombo;
    QVBoxLayout* m_cardsLayout;
    QWidget* m_cardsContainer;
    QList<Faq> m_faqsList;
};

// ABOUT & CONTACT WIDGET
class AboutWidget : public QWidget {
    Q_OBJECT

public:
    explicit AboutWidget(QWidget* parent = nullptr);
};

#endif // NOTICESFAQWIDGETS_H
