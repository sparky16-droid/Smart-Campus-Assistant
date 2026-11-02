#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include "../src/core/CampusDatabase.h"
#include "../src/core/EntityFactory.h"
#include "../src/core/SearchStrategy.h"
#include "../src/core/Observer.h"

int main() {
    std::cout << "========================================================\n"
              << "   RUNNING C++ OOP CONCEPTS AUTOMATED VERIFICATION      \n"
              << "========================================================\n";

    // 1. Test Encapsulation & Invariant checking
    std::cout << "[1/7] Testing Encapsulation... ";
    auto building = EntityFactory::createBuilding(
        "test-bld", "Test Science Hall", "TSH", "Physics and Chemistry wing",
        18.52, 73.85, 3, 2020, true, 100, 100, 50, 50, "#00ff00"
    );
    assert(building->getId() == "test-bld");
    assert(building->getName() == "Test Science Hall");
    assert(building->getFloors() == 3);

    building->setFloors(5);
    assert(building->getFloors() == 5);

    try {
        building->setName("");
        assert(false && "Should have thrown invalid_argument for empty name");
    } catch (const std::invalid_argument&) {
        // Expected behavior: invariant enforced
    }
    std::cout << "PASSED!\n";

    // 2. Test Inheritance
    std::cout << "[2/7] Testing Inheritance... ";
    CampusEntity* basePtr = building.get();
    assert(basePtr->getId() == "test-bld");
    assert(basePtr->matchesKeyword("Science"));
    std::cout << "PASSED!\n";

    // 3. Test Dynamic Polymorphism
    std::cout << "[3/7] Testing Dynamic Polymorphism... ";
    std::vector<std::shared_ptr<ICampusEntity>> polymorphicList;
    polymorphicList.push_back(building);
    polymorphicList.push_back(EntityFactory::createNotice(
        "not-test", "Urgent Exam Notice", "Final timetable released", "Exam",
        NoticePriority::Urgent, true, "Registrar"
    ));
    polymorphicList.push_back(EntityFactory::createFaq(
        "faq-test", "Where is the library?", "Central Library building 4", "General",
        {"library", "books"}
    ));

    assert(polymorphicList[0]->getEntityType() == "Building");
    assert(polymorphicList[1]->getEntityType() == "Notice");
    assert(polymorphicList[2]->getEntityType() == "FAQ");

    // Runtime vtable dispatch
    for (const auto& entity : polymorphicList) {
        assert(!entity->getDetails().empty());
        assert(entity->calculateRelevance("test") >= 0.0);
    }
    std::cout << "PASSED!\n";

    // 4. Test Operator Overloading
    std::cout << "[4/7] Testing Operator Overloading... ";
    auto noticeUrgent = EntityFactory::createNotice("n1", "Urgent Alert", "content", "Alert", NoticePriority::Urgent, true, "Admin", 1000);
    auto noticeNormal = EntityFactory::createNotice("n2", "Normal Notice", "content", "General", NoticePriority::Normal, true, "Admin", 2000);

    // operator< should prioritize Urgent over Normal
    assert(*noticeUrgent < *noticeNormal);

    auto bld1 = EntityFactory::createBuilding("b1", "Alpha", "A", "", 0, 0, 1, 2000, true, 0, 0, 0, 0, "");
    auto bld2 = EntityFactory::createBuilding("b2", "Beta", "B", "", 0, 0, 1, 2000, true, 0, 0, 0, 0, "");
    assert(*bld1 < *bld2);
    std::cout << "PASSED!\n";

    // 5. Test Generic Template Repository
    std::cout << "[5/7] Testing Template Repository<T>... ";
    CampusRepository<BuildingEntity> repo;
    repo.add(building);
    repo.add(bld1);
    repo.add(bld2);
    assert(repo.count() == 3);

    auto filtered = repo.filter([](const BuildingEntity& b) {
        return b.getFloors() >= 3;
    });
    assert(filtered.size() == 1);
    assert(filtered[0]->getId() == "test-bld");
    std::cout << "PASSED!\n";

    // 6. Test Design Patterns (Factory, Singleton, Observer, Strategy)
    std::cout << "[6/7] Testing Design Patterns... ";
    auto& db = CampusDatabase::instance();
    db.initialize();
    assert(db.buildings().count() >= 8);
    assert(db.facilities().count() >= 13);
    assert(db.departments().count() >= 6);

    // Observer Pattern
    auto publisher = std::make_shared<CampusNoticePublisher>();
    auto auditLogger = std::make_shared<AuditLoggerObserver>("AUDIT");
    publisher->attach(auditLogger);
    publisher->notify(*noticeUrgent);
    assert(auditLogger->getLogs().size() == 1);

    // Strategy Pattern
    ExactMatchStrategy exactStrategy;
    assert(exactStrategy.score("TSH", *building) == 1.0);
    assert(exactStrategy.score("xyz", *building) == 0.0);
    std::cout << "PASSED!\n";

    // 7. Test Cross-Entity Search
    std::cout << "[7/7] Testing Polymorphic Multi-Entity Search... ";
    auto results = db.searchAll("library", 0.40);
    assert(!results.empty());
    std::cout << "PASSED! (Found " << results.size() << " campus items)\n";

    std::cout << "\n>>> ALL OOP CONCEPTS AND ARCHITECTURE TESTS PASSED SUCCESSFULLY! <<<\n";
    return 0;
}
