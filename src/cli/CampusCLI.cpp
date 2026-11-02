#include "CampusCLI.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

CampusCLI::CampusCLI()
    : m_db(CampusDatabase::instance()),
      m_dijkstra(m_graph),
      m_astar(m_graph),
      m_running(true)
{
    m_db.initialize();

    // Populate Trie with building codes and facility names
    for (const auto& b : m_db.buildings().getAll()) {
        m_trie.insert(b->getName(), b->getId(), 10);
        m_trie.insert(b->getCode(), b->getId(), 15);
    }
    for (const auto& f : m_db.facilities().getAll()) {
        m_trie.insert(f->getName(), f->getId(), 5);
        m_trie.insert(f->getCategory(), f->getId(), 3);
    }

    // Populate Inverted Index with FAQs and Notices
    for (const auto& faq : m_db.faqs().getAll()) {
        m_index.addDocument(faq->getId(), faq->getQuestion(), faq->getAnswer());
    }
    for (const auto& notice : m_db.notices().getAll()) {
        m_index.addDocument(notice->getId(), notice->getName(), notice->getContent());
    }
}

void CampusCLI::printBanner() const {
    std::cout << "\n"
              << "====================================================================\n"
              << "  ____                 _      ____                                 \n"
              << " / ___| _ __ ___   __ _| |_   / ___|__ _ _ __ ___  _ __  _   _ ___ \n"
              << " \\___ \\| '_ ` _ \\ / _` | __| | |   / _` | '_ ` _ \\| '_ \\| | | / __|\n"
              << "  ___) | | | | | | (_| | |_  | |__| (_| | | | | | | |_) | |_| \\__ \\\n"
              << " |____/|_| |_| |_|\\__,_|\\__|  \\____\\__,_|_| |_| |_| .__/ \\__,_|___/\n"
              << "                                                  |_|              \n"
              << "  AI-Assisted Campus Information & Navigation System (C++ OOP Core) \n"
              << "====================================================================\n";
}

void CampusCLI::showMenu() const {
    std::cout << "\n[MAIN NAVIGATION CONSOLE]\n"
              << "  1. Smart Campus Chatbot (Natural Language Query & Help)\n"
              << "  2. Shortest Path Navigation (Dijkstra / A* Turn-by-Turn)\n"
              << "  3. Campus Buildings Directory (Schematic Map Overlays)\n"
              << "  4. Campus Facilities & Services (13 Operational Amenities)\n"
              << "  5. Academic Departments & Degree Programs\n"
              << "  6. Faculty Directory & Cabin Office Hours\n"
              << "  7. Official Notice Bulletin Board (Urgent / High Priority)\n"
              << "  8. Campus Knowledge Base & FAQs\n"
              << "  9. OOP Architecture Demonstrator (Inheritance, Polymorphism, Abstraction, Encapsulation)\n"
              << " 10. Administrative Control Console (Protected Pin Entry)\n"
              << "  0. Exit Campus Assistant\n"
              << "--------------------------------------------------------------------\n"
              << "Enter your selection (0-10): ";
}

void CampusCLI::handleChatbot() {
    std::cout << "\n--- [CAMPUS AI CHATBOT] ---\n"
              << "Type your question in natural English (or 'back' to return to menu).\n"
              << "Examples:\n"
              << "  * 'Where is the library and what are the timings?'\n"
              << "  * 'Who is the HOD of Computer Engineering?'\n"
              << "  * 'How do I reach the cafeteria from main gate?'\n"
              << "  * 'Is the medical clinic open 24 hours?'\n";

    std::string line;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << "\nYou: ";
        if (!std::getline(std::cin, line) || line == "back" || line == "exit" || line == "0") {
            break;
        }
        if (line.empty()) continue;

        auto intentRes = m_classifier.classify(line);
        std::cout << "Assistant [Intent: " << intentRes.intentName 
                  << " | Confidence: " << std::fixed << std::setprecision(0) 
                  << (intentRes.confidence * 100.0) << "%]:\n";

        if (intentRes.intent == UserIntent::Greeting) {
            std::cout << "Hello! I am your Smart Campus Assistant. I can help you locate campus buildings, "
                      << "navigate the campus grounds, find faculty cabin rooms, check notices, and explore facilities. "
                      << "How may I help you today?\n";
            continue;
        }

        // Search inverted index for FAQs first
        auto searchResults = m_index.search(line, 2);
        if (!searchResults.empty() && searchResults[0].tfIdfScore > 0.35) {
            auto faq = m_db.faqs().getById(searchResults[0].docId);
            if (faq) {
                faq->incrementViewCount();
                std::cout << "[FAQ Match]: " << faq->getQuestion() << "\n"
                          << faq->getAnswer() << "\n";
                continue;
            }
            auto notice = m_db.notices().getById(searchResults[0].docId);
            if (notice) {
                std::cout << "[Official Notice - " << notice->getPriorityString() << "]: " 
                          << notice->getName() << "\n" << notice->getContent() << "\n";
                continue;
            }
        }

        // Polymorphic cross-entity search
        auto entityMatches = m_db.searchAll(line, 0.45);
        if (!entityMatches.empty()) {
            std::cout << "Found relevant campus information:\n";
            for (size_t i = 0; i < std::min<size_t>(entityMatches.size(), 2); ++i) {
                std::cout << "\n" << entityMatches[i]->getDetails() << "\n";
            }
        } else {
            std::cout << "I couldn't locate an exact campus match for your query. "
                      << "Please try searching for a building code (e.g. 'LIB', 'ADM', 'ENG-A'), "
                      << "a facility like 'cafeteria' or 'gym', or ask a staff member at the Admin Block.\n";
        }
    }
}

void CampusCLI::handleNavigation() {
    std::cout << "\n--- [CAMPUS NAVIGATION & SHORTEST PATH FINDER] ---\n"
              << "Available Waypoints in Campus Graph:\n"
              << "  1. GATE_MAIN          (Campus Main Security Gate)\n"
              << "  2. NODE_ADM           (Administrative Block)\n"
              << "  3. NODE_LIB           (Central Library)\n"
              << "  4. NODE_ENG_A         (Academic Block A - Engineering)\n"
              << "  5. NODE_ENG_B         (Academic Block B - Sciences)\n"
              << "  6. NODE_SAC           (Student Activity Center & Cafeteria)\n"
              << "  7. NODE_SPT           (Indoor Sports Arena & Gym)\n"
              << "  8. NODE_HST           (Hostel Complex)\n"
              << "  9. NODE_MED           (Health Care Center & Pharmacy)\n"
              << " 10. NODE_CENTRAL_PLAZA (Central Fountain Plaza)\n\n";

    std::cout << "Enter Start Node ID (e.g. GATE_MAIN): ";
    std::string startId;
    std::cin >> startId;

    std::cout << "Enter Destination Node ID (e.g. NODE_LIB): ";
    std::string destId;
    std::cin >> destId;

    std::cout << "Require Wheelchair-Accessible Ramps/Elevators only? (y/n): ";
    char accChoice;
    std::cin >> accChoice;
    bool wheelchairOnly = (accChoice == 'y' || accChoice == 'Y');

    std::cout << "\nComputing shortest path via Dijkstra's Algorithm...\n";
    auto route = m_dijkstra.findShortestPath(startId, destId, wheelchairOnly);
    std::cout << route.getFormattedRoute();
}

void CampusCLI::handleBuildings() {
    std::cout << "\n--- [CAMPUS BUILDINGS DIRECTORY] ---\n";
    auto blds = m_db.buildings().getAll();
    for (size_t i = 0; i < blds.size(); ++i) {
        std::cout << " " << (i + 1) << ". ";
        blds[i]->printSummary(std::cout);
        std::cout << "\n";
    }

    std::cout << "\nEnter Building number to view full specifications (0 to back): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= static_cast<int>(blds.size())) {
        std::cout << "\n" << blds[choice - 1]->getDetails() << "\n";
    }
}

void CampusCLI::handleFacilities() {
    std::cout << "\n--- [CAMPUS FACILITIES & AMENITIES DIRECTORY] ---\n";
    auto facs = m_db.facilities().getAll();
    for (size_t i = 0; i < facs.size(); ++i) {
        std::cout << " " << (i + 1) << ". ";
        facs[i]->printSummary(std::cout);
        std::cout << "\n";
    }

    std::cout << "\nEnter Facility number for location and contact info (0 to back): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= static_cast<int>(facs.size())) {
        std::cout << "\n" << facs[choice - 1]->getDetails() << "\n";
    }
}

void CampusCLI::handleDepartments() {
    std::cout << "\n--- [ACADEMIC DEPARTMENTS DIRECTORY] ---\n";
    auto depts = m_db.departments().getAll();
    for (size_t i = 0; i < depts.size(); ++i) {
        std::cout << " " << (i + 1) << ". ";
        depts[i]->printSummary(std::cout);
        std::cout << "\n";
    }

    std::cout << "\nEnter Department number for degree programs & faculty roster (0 to back): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= static_cast<int>(depts.size())) {
        std::cout << "\n" << depts[choice - 1]->getDetails() << "\n";
    }
}

void CampusCLI::handleFaculty() {
    std::cout << "\n--- [FACULTY DIRECTORY & OFFICE HOURS] ---\n";
    auto facs = m_db.faculty().getAll();
    for (size_t i = 0; i < facs.size(); ++i) {
        std::cout << " " << (i + 1) << ". ";
        facs[i]->printSummary(std::cout);
        std::cout << "\n";
    }

    std::cout << "\nEnter Professor number for cabin location & research info (0 to back): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= static_cast<int>(facs.size())) {
        std::cout << "\n" << facs[choice - 1]->getDetails() << "\n";
    }
}

void CampusCLI::handleNotices() {
    std::cout << "\n--- [OFFICIAL NOTICE BULLETIN BOARD] ---\n";
    // Sorted by urgency: Urgent -> High -> Normal
    auto sortedNotices = m_db.notices().getSorted([](const NoticeEntity& a, const NoticeEntity& b) {
        return a < b;
    });

    for (size_t i = 0; i < sortedNotices.size(); ++i) {
        std::cout << " " << (i + 1) << ". ";
        sortedNotices[i]->printSummary(std::cout);
        std::cout << "\n";
    }

    std::cout << "\nEnter Notice number to read full circular text (0 to back): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= static_cast<int>(sortedNotices.size())) {
        std::cout << "\n" << sortedNotices[choice - 1]->getDetails() << "\n";
    }
}

void CampusCLI::handleFaqs() {
    std::cout << "\n--- [CAMPUS KNOWLEDGE BASE & FAQS] ---\n";
    auto faqs = m_db.faqs().getAll();
    for (size_t i = 0; i < faqs.size(); ++i) {
        std::cout << " " << (i + 1) << ". [" << faqs[i]->getCategory() << "] " 
                  << faqs[i]->getQuestion() << "\n";
    }

    std::cout << "\nEnter FAQ number to view official answer (0 to back): ";
    int choice;
    std::cin >> choice;
    if (choice >= 1 && choice <= static_cast<int>(faqs.size())) {
        faqs[choice - 1]->incrementViewCount();
        std::cout << "\n" << faqs[choice - 1]->getDetails() << "\n";
    }
}

void CampusCLI::handleOopDemonstration() {
    std::cout << "\n====================================================================\n"
              << "   OBJECT-ORIENTED PROGRAMMING (OOP) ARCHITECTURE DEMONSTRATOR     \n"
              << "   Presentation Guide for Faculty Examiners & Project Evaluators    \n"
              << "====================================================================\n\n"
              << "1. ENCAPSULATION:\n"
              << "   * Class: CampusEntity (src/core/CampusEntity.h)\n"
              << "   * Evidence: Strict private members (m_id, m_name, m_code, m_description, timestamps).\n"
              << "   * Invariant Enforcement: Validated setters throw std::invalid_argument if invalid.\n\n"
              << "2. INHERITANCE:\n"
              << "   * Classes: BuildingEntity, FacilityEntity, DepartmentEntity, FacultyEntity, NoticeEntity\n"
              << "   * Evidence: public inheritance (class BuildingEntity : public CampusEntity).\n"
              << "   * Reuses identity, code, timestamps, and search token generators.\n\n"
              << "3. POLYMORPHISM (Dynamic & Static):\n"
              << "   * Dynamic Polymorphism: ICampusEntity (pure abstract interface). Vector of shared_ptr<ICampusEntity>\n"
              << "     invokes runtime-specific getDetails(), calculateRelevance(), serializeJson().\n"
              << "   * Static Polymorphism: Operator overloading (operator<<, operator==, operator<).\n\n"
              << "4. DATA ABSTRACTION:\n"
              << "   * Pure abstract base interface: ICampusEntity (src/core/ICampusEntity.h).\n"
              << "   * Exposes uniform contract for heterogeneous campus elements.\n\n"
              << "5. TEMPLATES & GENERIC PROGRAMMING:\n"
              << "   * Template Class: CampusRepository<T> (src/core/CampusRepository.h).\n"
              << "   * Type-safe CRUD, lambda predicates, and thread-safe mutex locks.\n\n"
              << "6. DESIGN PATTERNS IMPLEMENTED:\n"
              << "   * Factory Pattern: EntityFactory (src/core/EntityFactory.h)\n"
              << "   * Singleton Pattern: CampusDatabase (src/core/CampusDatabase.h)\n"
              << "   * Strategy Pattern: ISearchStrategy (src/core/SearchStrategy.h)\n"
              << "   * Observer Pattern: CampusNoticePublisher (src/core/Observer.h)\n"
              << "====================================================================\n";
}

void CampusCLI::handleAdminPortal() {
    std::cout << "\n--- [CAMPUS ADMINISTRATIVE CONSOLE] ---\n"
              << "Enter Admin Username: ";
    std::string user;
    std::cin >> user;

    std::cout << "Enter Admin Password: ";
    std::string pass;
    std::cin >> pass;

    if (!m_db.verifyAdmin(user, pass)) {
        std::cout << "[ERROR] Access Denied: Invalid administrator credentials.\n";
        return;
    }

    std::cout << "\n[AUTHENTICATION SUCCESSFUL - WELCOME " << user << "]\n"
              << m_db.getSystemStats() << "\n"
              << "In-Memory Graph Nodes: " << m_graph.getNodeCount() << "\n"
              << "Trie Indexed Keywords: Ready for Autocomplete\n"
              << "Inverted Index Vocabulary: " << m_index.getVocabularySize() << " terms indexed\n";
}

void CampusCLI::run() {
    printBanner();

    while (m_running) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:  handleChatbot(); break;
            case 2:  handleNavigation(); break;
            case 3:  handleBuildings(); break;
            case 4:  handleFacilities(); break;
            case 5:  handleDepartments(); break;
            case 6:  handleFaculty(); break;
            case 7:  handleNotices(); break;
            case 8:  handleFaqs(); break;
            case 9:  handleOopDemonstration(); break;
            case 10: handleAdminPortal(); break;
            case 0:
                std::cout << "\nExiting Smart Campus Assistant. Have a productive day on campus!\n";
                m_running = false;
                break;
            default:
                std::cout << "\n[!] Invalid option. Please enter a valid menu number (0-10).\n";
                break;
        }
    }
}
