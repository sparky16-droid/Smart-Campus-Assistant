#include <iostream>
#include <cassert>
#include "../src/analytics/CampusAnalytics.h"
#include "../src/navigation/GeoSpatialUtils.h"
#include "../src/core/SerializationEngine.h"
#include "../src/core/CampusDatabase.h"

int main() {
    std::cout << "========================================================\n"
              << "   RUNNING C++ ANALYTICS & GEOSPATIAL TEST SUITE        \n"
              << "========================================================\n";

    // 1. Test Geodesic Haversine Distance
    std::cout << "[1/4] Testing Haversine Geodesic Distance... ";
    GeoCoordinate gate(18.52043, 73.85674);
    GeoCoordinate library(18.52080, 73.85620);
    double dist = GeoSpatialUtils::haversineDistanceMeters(gate, library);
    assert(dist > 50.0 && dist < 150.0);
    double bearing = GeoSpatialUtils::initialBearingDegrees(gate, library);
    assert(bearing >= 0.0 && bearing < 360.0);
    std::string heading = GeoSpatialUtils::bearingToCompassDirection(bearing);
    assert(!heading.empty());
    std::cout << "PASSED! (Distance: " << dist << "m, Compass: " << heading << ")\n";

    // 2. Test Campus Analytics
    std::cout << "[2/4] Testing Campus Analytics & Statistics... ";
    CampusAnalytics analytics;
    analytics.logQuery("where is library", "Navigation", 0.95, true);
    analytics.logQuery("gym timings", "FacilitySearch", 0.88, true);
    analytics.logQuery("who is hod", "FacultySearch", 0.92, true);
    analytics.logQuery("random gibberish", "GeneralQuery", 0.30, false);
    analytics.recordBuildingLookup("LIB");
    analytics.recordBuildingLookup("LIB");
    analytics.recordBuildingLookup("ADM");

    assert(analytics.getResolutionRate() == 0.75); // 3 of 4 resolved
    auto stats = analytics.calculateConfidenceStats();
    assert(stats.sampleCount == 4);
    assert(stats.minVal == 0.30);
    assert(stats.maxVal == 0.95);
    std::cout << "PASSED!\n";

    // 3. Test Serialization Engine
    std::cout << "[3/4] Testing JSON and CSV Serialization... ";
    auto& db = CampusDatabase::instance();
    db.initialize();

    std::string jsonBlds = SerializationEngine::exportBuildingsToJson(db.buildings().getAll());
    assert(jsonBlds.find("ADM") != std::string::npos);
    assert(jsonBlds.find("LIB") != std::string::npos);

    std::string csvFacs = SerializationEngine::exportFacilitiesToCsv(db.facilities().getAll());
    assert(csvFacs.find("Central Library") != std::string::npos);
    std::cout << "PASSED!\n";

    // 4. Test Audit Report Generation
    std::cout << "[4/4] Testing Audit Report Formatting... ";
    std::string report = analytics.generateAuditReport();
    assert(report.find("SMART CAMPUS ANALYTICS") != std::string::npos);
    assert(report.find("Resolution Success Rate") != std::string::npos);
    std::cout << "PASSED!\n";

    std::cout << "\n>>> ALL ANALYTICS AND GEOSPATIAL TESTS PASSED SUCCESSFULLY! <<<\n";
    return 0;
}
