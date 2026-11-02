#include "GeoSpatialUtils.h"

double GeoSpatialUtils::haversineDistanceMeters(const GeoCoordinate& p1, const GeoCoordinate& p2) {
    if (!p1.isValid() || !p2.isValid()) return 0.0;

    double lat1Rad = toRadians(p1.latitude);
    double lat2Rad = toRadians(p2.latitude);
    double dLat = toRadians(p2.latitude - p1.latitude);
    double dLon = toRadians(p2.longitude - p1.longitude);

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::cos(lat1Rad) * std::cos(lat2Rad) *
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0);

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return EARTH_RADIUS_METERS * c;
}

double GeoSpatialUtils::initialBearingDegrees(const GeoCoordinate& from, const GeoCoordinate& to) {
    if (!from.isValid() || !to.isValid()) return 0.0;

    double lat1 = toRadians(from.latitude);
    double lat2 = toRadians(to.latitude);
    double dLon = toRadians(to.longitude - from.longitude);

    double y = std::sin(dLon) * std::cos(lat2);
    double x = std::cos(lat1) * std::sin(lat2) -
               std::sin(lat1) * std::cos(lat2) * std::cos(dLon);

    double bearing = toDegrees(std::atan2(y, x));
    bearing = std::fmod(bearing + 360.0, 360.0);
    return bearing;
}

std::string GeoSpatialUtils::bearingToCompassDirection(double bearingDegrees) {
    static const std::string directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    int index = static_cast<int>((bearingDegrees + 22.5) / 45.0) % 8;
    return directions[index];
}

double GeoSpatialUtils::planarDistanceMeters(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}
