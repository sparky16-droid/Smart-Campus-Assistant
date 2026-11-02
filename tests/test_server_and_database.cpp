#include <iostream>
#include <cassert>
#include "../src/database/SqliteBridge.h"
#include "../src/server/NativeHttpServer.h"
#include "../src/models/CourseEntity.h"
#include "../src/models/EventEntity.h"

int main() {
    std::cout << "========================================================\n"
              << "   RUNNING C++ SERVER, DATABASE & MODELS TEST SUITE     \n"
              << "========================================================\n";

    // 1. Test SQL Query Builder
    std::cout << "[1/4] Testing Fluent SQL Query Builder... ";
    SelectQueryBuilder select("buildings");
    select.select({"id", "code", "name", "floors"})
          .where("wheelchairAccessible", SqlConditionOp::Equal, "1")
          .orderBy("name", true)
          .limit(10);
    std::string sql = select.build();
    assert(sql.find("SELECT id, code, name, floors FROM buildings") != std::string::npos);
    assert(sql.find("WHERE wheelchairAccessible = '1'") != std::string::npos);
    assert(sql.find("LIMIT 10") != std::string::npos);

    InsertQueryBuilder insert("facilities");
    insert.set("id", "fac-100")
          .set("name", "New Innovation Lab")
          .set("isOperational", 1);
    std::string insertSql = insert.build();
    assert(insertSql.find("INSERT INTO facilities") != std::string::npos);
    assert(insertSql.find("New Innovation Lab") != std::string::npos);
    std::cout << "PASSED!\n";

    // 2. Test HTTP Request Parser & Router Dispatcher
    std::cout << "[2/4] Testing Native HTTP Parser & Router... ";
    std::string rawReq = "GET /api/buildings?filter=accessible HTTP/1.1\r\n"
                         "Host: localhost:8080\r\n"
                         "User-Agent: CppTestClient/1.0\r\n\r\n";
    HttpRequest parsedReq = HttpRequest::parse(rawReq);
    assert(parsedReq.method == HttpMethod::GET);
    assert(parsedReq.path == "/api/buildings");
    assert(parsedReq.queryParams["filter"] == "accessible");
    assert(parsedReq.headers["Host"] == "localhost:8080");

    HttpRouter router;
    router.registerCampusApiEndpoints();
    HttpResponse res = router.dispatch(parsedReq);
    assert(res.statusCode == 200);
    assert(!res.body.empty());
    assert(res.headers["Content-Type"].find("application/json") != std::string::npos);
    std::cout << "PASSED! (HTTP 200 OK, Dispatched " << res.body.size() << " bytes)\n";

    // 3. Test Course Entity
    std::cout << "[3/4] Testing Course Entity Hierarchy... ";
    CourseEntity course("c-101", "Object-Oriented Programming with C++", "CS-201",
                         "Core principles of OOP including encapsulation, inheritance, polymorphism, templates, and STL.",
                         "dept-1", "Computer Engineering", 4, 3,
                         {"CS-101 Programming Fundamentals"},
                         {"Classes & Objects", "Inheritance & Dynamic Polymorphism", "Templates & STL", "Design Patterns"},
                         false);
    assert(course.getId() == "c-101");
    assert(course.getCode() == "CS-201");
    assert(course.getCredits() == 4);
    assert(course.getPrerequisites().size() == 1);
    assert(course.getSyllabus().size() == 4);
    assert(course.calculateRelevance("C++") > 0.80);
    std::cout << "PASSED!\n";

    // 4. Test Event Entity
    std::cout << "[4/4] Testing Event Entity Registration... ";
    EventEntity event("ev-1", "TechnoVision 24-Hour Hackathon", "Annual inter-college software hackathon",
                      "bld-1", "Administrative Block", "Main Auditorium",
                      "Saturday 9:00 AM", "Coding Club", "Dr. Rajeshwar Sharma", 200, 0.0);
    assert(event.getName() == "TechnoVision 24-Hour Hackathon");
    assert(event.getMaxCapacity() == 200);
    assert(!event.isSoldOut());
    bool regOk = event.registerAttendee();
    assert(regOk);
    assert(event.getRegisteredCount() == 1);
    std::cout << "PASSED!\n";

    std::cout << "\n>>> ALL SERVER, DATABASE & DOMAIN MODEL TESTS PASSED SUCCESSFULLY! <<<\n";
    return 0;
}
