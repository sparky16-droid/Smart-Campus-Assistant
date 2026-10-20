#include "ChatbotEngine.h"
#include "SynonymEngine.h"
#include "../database/DatabaseManager.h"
#include <QRegularExpression>
#include <QDateTime>

ChatbotEngine& ChatbotEngine::instance() {
    static ChatbotEngine instance;
    return instance;
}

ChatbotEngine::ChatbotEngine() {
    QStringList greets = {
        "hi", "hello", "hey", "greetings", "good morning",
        "good afternoon", "good evening", "namaste", "howdy", "sup", "welcome"
    };
    for (const auto& g : greets) {
        m_greetings.insert(g);
    }
}

void ChatbotEngine::clearSession(const QString& sessionId) {
    m_sessionContext.remove(sessionId);
}

bool ChatbotEngine::isGreeting(const QString& text) const {
    QString clean = text.toLower().trimmed();
    clean.remove(QRegularExpression("[^a-z\\s]"));
    if (m_greetings.contains(clean)) return true;

    QStringList words = clean.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    return (words.size() <= 2 && !words.isEmpty() && m_greetings.contains(words.first()));
}

QString ChatbotEngine::resolveContext(const QString& query, const QString& sessionId) {
    if (!m_sessionContext.contains(sessionId)) return query;

    QString lower = query.toLower();
    bool hasPronoun = lower.contains(" it") || lower.startsWith("it") ||
                      lower.contains(" there") ||
                      lower.contains("timings") || lower.contains("timing") ||
                      lower.contains("hours") || lower.contains("contact") ||
                      lower.contains("phone") || lower.contains("email") ||
                      lower.contains("how to get") || lower.contains("directions") ||
                      lower.contains("where is that") || lower.contains("hod");

    if (hasPronoun) {
        const auto& ref = m_sessionContext.value(sessionId);
        if (!ref.name.isEmpty()) {
            return query + " " + ref.name + " " + ref.buildingName;
        }
    }
    return query;
}

int ChatbotEngine::calculateScore(const QString& targetText, const QSet<QString>& expandedTokens, const QString& rawPhrase) const {
    QString lower = targetText.toLower();
    int score = 0;

    // Exact phrase bonus
    if (rawPhrase.length() > 3 && lower.contains(rawPhrase.toLower())) {
        score += 12;
    }

    // Token overlap
    for (const auto& tok : expandedTokens) {
        if (lower.contains(tok)) {
            score += (tok.length() > 4 ? 3 : 2);
        }
    }
    return score;
}

ChatbotAnswer ChatbotEngine::processQuery(const QString& rawMessage, const QString& sessionId) {
    QString trimmed = rawMessage.trimmed();
    ChatbotAnswer ans;

    if (trimmed.isEmpty()) {
        ans.answerText = "Please enter your campus question (e.g., 'Where is the library?', 'Where is the cafeteria?', or 'Who is the HOD of Computer Science?').";
        ans.confidence = 1.0;
        ans.source = "fallback";
        ans.suggestedQuestions = {
            "Where is the library?",
            "Where is the cafeteria?",
            "How can I reach the Computer Science department?",
            "What are the clinic hours?"
        };
        return ans;
    }

    // 1. Save user query in SQLite
    DatabaseManager::instance().saveChatMessage(sessionId, "user", trimmed);

    // 2. Greetings check
    if (isGreeting(trimmed)) {
        ans.answerText = "Hello! 👋 I am your **Smart Campus Assistant**.\n\n"
                         "I can help you locate campus buildings, faculty offices, departments, cafeteria timings, examination notices, and answer frequently asked questions.\n\n"
                         "How may I assist you today?";
        ans.confidence = 0.99;
        ans.source = "greeting";
        ans.suggestedQuestions = {
            "Where is the Central Library?",
            "Where is the college cafeteria?",
            "How to contact the Admissions Office?",
            "Who is the HOD of Computer Science?",
            "Where are the ATMs located?"
        };
        ans.messageId = DatabaseManager::instance().saveChatMessage(sessionId, "assistant", ans.answerText);
        return ans;
    }

    // 3. Resolve Pronouns / Follow-ups from Active Session Context
    QString resolved = resolveContext(trimmed, sessionId);
    QStringList tokens = SynonymEngine::instance().tokenize(resolved);
    QSet<QString> expandedTokens = SynonymEngine::instance().expandTokens(tokens);
    QString cleanedPhrase = trimmed.toLower().remove(QRegularExpression("[^a-z0-9\\s]")).trimmed();

    auto& db = DatabaseManager::instance();

    // 4. Match FAQs
    QList<Faq> faqs = db.getAllFaqs();
    Faq bestFaq;
    int bestFaqScore = 0;
    for (const auto& f : faqs) {
        QString combined = f.question + " " + f.keywords + " " + f.answer + " " + f.category;
        int s = calculateScore(combined, expandedTokens, cleanedPhrase);
        if (s > bestFaqScore) {
            bestFaqScore = s;
            bestFaq = f;
        }
    }

    // 5. Match Facilities
    QList<Facility> facilities = db.getAllFacilities();
    Facility bestFac;
    int bestFacScore = 0;
    for (const auto& fac : facilities) {
        QString combined = fac.name + " " + fac.category + " " + fac.description + " " + fac.landmark + " " + fac.floor + " " + fac.buildingName;
        int s = calculateScore(combined, expandedTokens, cleanedPhrase);
        if (s > bestFacScore) {
            bestFacScore = s;
            bestFac = fac;
        }
    }

    // 6. Match Departments
    QList<Department> depts = db.getAllDepartments();
    Department bestDept;
    int bestDeptScore = 0;
    for (const auto& d : depts) {
        QString combined = d.name + " " + d.code + " " + d.headOfDepartment + " " + d.description + " " + d.programsOffered;
        int s = calculateScore(combined, expandedTokens, cleanedPhrase);
        if (s > bestDeptScore) {
            bestDeptScore = s;
            bestDept = d;
        }
    }

    // 7. Match Faculty
    QList<Faculty> facultyList = db.getAllFaculty();
    Faculty bestFaculty;
    int bestFacultyScore = 0;
    for (const auto& fac : facultyList) {
        QString combined = fac.name + " " + fac.designation + " " + fac.specialization + " " + fac.cabinRoom + " " + fac.departmentName;
        int s = calculateScore(combined, expandedTokens, cleanedPhrase);
        if (s > bestFacultyScore) {
            bestFacultyScore = s;
            bestFaculty = fac;
        }
    }

    // 8. Match Notices
    QList<Notice> notices = db.getAllNotices();
    Notice bestNotice;
    int bestNoticeScore = 0;
    for (const auto& n : notices) {
        QString combined = n.title + " " + n.category + " " + n.content;
        int s = calculateScore(combined, expandedTokens, cleanedPhrase);
        if (s > bestNoticeScore) {
            bestNoticeScore = s;
            bestNotice = n;
        }
    }

    // Determine highest score
    int maxScore = qMax(bestFaqScore, qMax(bestFacScore, qMax(bestDeptScore, qMax(bestFacultyScore, bestNoticeScore))));

    // Anti-hallucination threshold
    if (maxScore < 4) {
        ans.answerText = "I couldn't find verified information about that yet in the campus knowledge base.\n\n"
                         "You can check our verified campus directories or contact the central administration:\n"
                         "• 📞 **Helpline:** +91 (080) 4123-4567\n"
                         "• ✉️ **Email:** assistant@smartcampus.edu\n"
                         "• 🏢 **Office:** Administration Complex (ADM-1), Ground Floor";
        ans.confidence = 0.2;
        ans.source = "fallback";
        ans.suggestedQuestions = {
            "Where is the library?",
            "Where is the cafeteria?",
            "How can I reach the Computer Science department?",
            "What are the clinic timings?",
            "View upcoming examination notices"
        };
        ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText);
        return ans;
    }

    // Facility match
    if (maxScore == bestFacScore && !bestFac.id.isEmpty()) {
        ans.source = "facility";
        ans.confidence = qMin(0.95, bestFacScore / 15.0);

        ans.reference.type = "facility";
        ans.reference.id = bestFac.id;
        ans.reference.name = bestFac.name;
        ans.reference.buildingName = bestFac.buildingName;
        ans.reference.floor = bestFac.floor;
        ans.reference.roomNumber = bestFac.roomNumber;
        ans.reference.timings = bestFac.timings;
        ans.reference.linkUrl = "/facilities/" + bestFac.id;

        m_sessionContext[sessionId] = ans.reference;

        ans.answerText = QString("### 📍 %1\n\n"
                                 "**Category:** %2\n"
                                 "**Building:** %3\n"
                                 "%4"
                                 "%5"
                                 "%6"
                                 "%7\n"
                                 "%8\n\n"
                                 "🧭 *You can locate this place on the Campus Map tab or click 'Show on Map'.*")
                         .arg(bestFac.name)
                         .arg(bestFac.category)
                         .arg(bestFac.buildingName.isEmpty() ? "Campus Complex" : bestFac.buildingName)
                         .arg(bestFac.floor.isEmpty() ? "" : QString("**Floor/Room:** %1%2\n").arg(bestFac.floor).arg(bestFac.roomNumber.isEmpty() ? "" : ", " + bestFac.roomNumber))
                         .arg(bestFac.landmark.isEmpty() ? "" : QString("**Landmark:** %1\n").arg(bestFac.landmark))
                         .arg(bestFac.timings.isEmpty() ? "" : QString("**Timings:** ⏰ %1\n").arg(bestFac.timings))
                         .arg(bestFac.contactPhone.isEmpty() ? "" : QString("**Contact:** 📞 %1\n").arg(bestFac.contactPhone))
                         .arg(bestFac.description);

        ans.suggestedQuestions = {
            QString("What are the timings for %1?").arg(bestFac.name),
            QString("Show %1 on map").arg(bestFac.name),
            "Where is the cafeteria?",
            "Where is the central library?"
        };

        ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText, ans.reference.name);
        return ans;
    }

    // FAQ match
    if (maxScore == bestFaqScore && !bestFaq.id.isEmpty()) {
        ans.source = "faq";
        ans.confidence = qMin(0.98, bestFaqScore / 14.0);
        ans.answerText = bestFaq.answer;
        db.incrementFaqView(bestFaq.id);

        ans.reference.type = "faq";
        ans.reference.id = bestFaq.id;
        ans.reference.name = bestFaq.question;

        m_sessionContext[sessionId] = ans.reference;

        ans.suggestedQuestions = {
            "What are the library timings?",
            "Where is the cafeteria?",
            "Where is the administration office?",
            "Show me latest notices"
        };

        ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText);
        return ans;
    }

    // Department match
    if (maxScore == bestDeptScore && !bestDept.id.isEmpty()) {
        ans.source = "department";
        ans.confidence = qMin(0.95, bestDeptScore / 15.0);

        ans.reference.type = "department";
        ans.reference.id = bestDept.id;
        ans.reference.name = bestDept.name;
        ans.reference.buildingName = bestDept.buildingName;

        m_sessionContext[sessionId] = ans.reference;

        ans.answerText = QString("### 🎓 Department of %1 (%2)\n\n"
                                 "**Head of Department:** %3\n"
                                 "**Location:** %4\n"
                                 "**Email:** ✉️ %5\n"
                                 "%6"
                                 "**Programs Offered:** %7\n\n"
                                 "%8")
                         .arg(bestDept.name)
                         .arg(bestDept.code)
                         .arg(bestDept.headOfDepartment)
                         .arg(bestDept.buildingName.isEmpty() ? "Academic Complex" : bestDept.buildingName)
                         .arg(bestDept.contactEmail)
                         .arg(bestDept.contactPhone.isEmpty() ? "" : QString("**Phone:** 📞 %1\n").arg(bestDept.contactPhone))
                         .arg(bestDept.programsOffered)
                         .arg(bestDept.description);

        ans.suggestedQuestions = {
            QString("Who is the HOD of %1?").arg(bestDept.code),
            QString("Where is the %1 department located?").arg(bestDept.code),
            "View faculty directory",
            "Where is the computer lab?"
        };

        ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText, ans.reference.name);
        return ans;
    }

    // Faculty match
    if (maxScore == bestFacultyScore && !bestFaculty.id.isEmpty()) {
        ans.source = "faculty";
        ans.confidence = qMin(0.95, bestFacultyScore / 15.0);

        ans.reference.type = "faculty";
        ans.reference.id = bestFaculty.id;
        ans.reference.name = bestFaculty.title + " " + bestFaculty.name;
        ans.reference.buildingName = bestFaculty.departmentName;
        ans.reference.roomNumber = bestFaculty.cabinRoom;

        m_sessionContext[sessionId] = ans.reference;

        ans.answerText = QString("### 👨‍🏫 %1 %2\n\n"
                                 "**Designation:** %3\n"
                                 "**Department:** %4\n"
                                 "%5"
                                 "%6"
                                 "**Email:** ✉️ %7\n"
                                 "%8"
                                 "%9\n"
                                 "%10")
                         .arg(bestFaculty.title)
                         .arg(bestFaculty.name)
                         .arg(bestFaculty.designation)
                         .arg(bestFaculty.departmentName)
                         .arg(bestFaculty.cabinRoom.isEmpty() ? "" : QString("**Cabin / Room:** 🚪 %1\n").arg(bestFaculty.cabinRoom))
                         .arg(bestFaculty.officeHours.isEmpty() ? "" : QString("**Office Hours:** ⏰ %1\n").arg(bestFaculty.officeHours))
                         .arg(bestFaculty.email)
                         .arg(bestFaculty.phone.isEmpty() ? "" : QString("**Phone:** 📞 %1\n").arg(bestFaculty.phone))
                         .arg(bestFaculty.specialization.isEmpty() ? "" : QString("**Specialization:** %1\n").arg(bestFaculty.specialization))
                         .arg(bestFaculty.bio.isEmpty() ? "" : QString("*%1*").arg(bestFaculty.bio));

        ans.suggestedQuestions = {
            QString("Where is %1's office?").arg(bestFaculty.name),
            QString("How to contact %1?").arg(bestFaculty.name),
            "View all faculty members"
        };

        ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText, ans.reference.name);
        return ans;
    }

    // Notice match
    if (maxScore == bestNoticeScore && !bestNotice.id.isEmpty()) {
        ans.source = "notice";
        ans.confidence = qMin(0.92, bestNoticeScore / 15.0);

        ans.answerText = QString("### 📢 Notice: %1\n\n"
                                 "**Category:** %2 | **Priority:** %3 | **Date:** %4\n"
                                 "**Issued by:** %5\n\n"
                                 "%6")
                         .arg(bestNotice.title)
                         .arg(bestNotice.category)
                         .arg(bestNotice.priority)
                         .arg(bestNotice.publishDate.toString("MMMM dd, yyyy"))
                         .arg(bestNotice.author)
                         .arg(bestNotice.content);

        ans.suggestedQuestions = {
            "What are the upcoming exams?",
            "View all recent notices",
            "Where is the examination office?"
        };

        ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText);
        return ans;
    }

    // Fallback
    ans.answerText = "I couldn't find verified information about that yet in the campus knowledge base.\n\nYou can check the campus directory or contact the administration office.";
    ans.confidence = 0.3;
    ans.source = "fallback";
    ans.messageId = db.saveChatMessage(sessionId, "assistant", ans.answerText);
    return ans;
}
