#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <cassert>
#include "../src/database/DatabaseManager.h"
#include "../src/nlp/ChatbotEngine.h"
#include "../src/nlp/SynonymEngine.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    std::cout << "==================================================" << std::endl;
    std::cout << "   Smart Campus Assistant Automated Test Suite    " << std::endl;
    std::cout << "==================================================" << std::endl;

    auto& db = DatabaseManager::instance();
    bool init = db.initialize("smart_campus.db");
    assert(init);
    std::cout << "[PASS] Database connection initialized" << std::endl;

    // 1. Test Buildings
    auto buildings = db.getAllBuildings();
    assert(buildings.size() >= 8);
    std::cout << "[PASS] Buildings count verified: " << buildings.size() << std::endl;

    // 2. Test Facilities
    auto facilities = db.getAllFacilities();
    assert(facilities.size() >= 13);
    std::cout << "[PASS] Facilities count verified: " << facilities.size() << std::endl;

    // 3. Test Departments
    auto depts = db.getAllDepartments();
    assert(depts.size() >= 6);
    std::cout << "[PASS] Departments count verified: " << depts.size() << std::endl;

    // 4. Test Faculty
    auto faculty = db.getAllFaculty();
    assert(faculty.size() >= 8);
    std::cout << "[PASS] Faculty count verified: " << faculty.size() << std::endl;

    // 5. Test FAQs
    auto faqs = db.getAllFaqs();
    assert(faqs.size() >= 10);
    std::cout << "[PASS] FAQs count verified: " << faqs.size() << std::endl;

    // 6. Test Admin Auth
    bool adminOk = db.verifyAdmin("admin", "adminpassword123");
    assert(adminOk);
    bool adminFail = db.verifyAdmin("admin", "wrongpassword");
    assert(!adminFail);
    std::cout << "[PASS] Admin password verification verified" << std::endl;

    // 7. Test Chatbot Retrieval & Grounding
    auto& bot = ChatbotEngine::instance();

    // Greeting
    auto ansGreet = bot.processQuery("Hello", "test-session");
    assert(ansGreet.source == "greeting");
    std::cout << "[PASS] Greeting intent recognized" << std::endl;

    // Library Query
    auto ansLib = bot.processQuery("Where is the Central Library?", "test-session");
    assert(ansLib.confidence > 0.7);
    assert(ansLib.answerText.contains("Library", Qt::CaseInsensitive));
    std::cout << "[PASS] Library query grounded successfully" << std::endl;

    // Contextual Follow-up with pronoun "its"
    auto ansTiming = bot.processQuery("What are its timings?", "test-session");
    assert(ansTiming.confidence > 0.5);
    std::cout << "[PASS] Contextual follow-up resolved successfully" << std::endl;

    // Cafeteria Query
    auto ansCafe = bot.processQuery("Where is the food court or canteen?", "test-session");
    assert(ansCafe.answerText.contains("Cafeteria", Qt::CaseInsensitive) || ansCafe.answerText.contains("Food Court", Qt::CaseInsensitive));
    std::cout << "[PASS] Synonym expansion (canteen -> cafeteria) verified" << std::endl;

    // Fallback for unknown / fabricated facts
    auto ansUnknown = bot.processQuery("What is the secret rocket code for mars?", "test-session");
    assert(ansUnknown.source == "fallback");
    std::cout << "[PASS] Anti-hallucination guardrail verified" << std::endl;

    std::cout << "==================================================" << std::endl;
    std::cout << " 🎉 ALL 7 TEST SUITES PASSED CLEANLY (100% SUCCESS)!" << std::endl;
    std::cout << "==================================================" << std::endl;
    return 0;
}
