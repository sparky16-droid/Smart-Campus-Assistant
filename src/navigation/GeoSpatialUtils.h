#ifndef GEOSPATIAL_UTILS_H
#define GEOSPATIAL_UTILS_H

#include <cmath>
#include <string>

struct GeoCoordinate {
    double latitude;
    double longitude;

    GeoCoordinate() : latitude(0.0), longitude(0.0) {}
    GeoCoordinate(double lat, double lon) : latitude(lat), longitude(lon) {}

    bool isValid() const {
        return latitude >= -90.0 && latitude <= 90.0 && longitude >= -180.0 && longitude <= 180.0;
    }
};

struct BoundingBox {
    GeoCoordinate southWest;
    GeoCoordinate northEast;

    bool contains(const GeoCoordinate& point) const {
        return point.latitude >= southWest.latitude && point.latitude <= northEast.latitude &&
               point.longitude >= southWest.longitude && point.longitude <= northEast.longitude;
    }
};

/**
 * @file GeoSpatialUtils.h
 * @brief Geodesic mathematics and spatial calculation utilities in C++.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: MATHEMATICAL UTILITIES & ENCAPSULATION
 * - Computes spherical Earth distance via the Haversine formula.
 * - Computes true compass bearing between campus coordinates.
 */
class GeoSpatialUtils {
public:
    static constexpr double EARTH_RADIUS_METERS = 6371000.0;
    static constexpr double PI = 3.14159265358979323846;

    static double toRadians(double degrees) {
        return degrees * (PI / 180.0);
    }

    static double toDegrees(double radians) {
        return radians * (180.0 / PI);
    }

    // Great-Circle distance using Haversine formula in meters
    static double haversineDistanceMeters(const GeoCoordinate& p1, const GeoCoordinate& p2);

    // Initial bearing in degrees [0, 360)
    static double initialBearingDegrees(const GeoCoordinate& from, const GeoCoordinate& to);

    // Cardinal compass heading ("N", "NE", "E", "SE", "S", "SW", "W", "NW")
    static std::string bearingToCompassDirection(double bearingDegrees);

    // Flat projection schematic distance (2D plane approximation)
    static double planarDistanceMeters(double x1, double y1, double x2, double y2);
};

#endif // GEOSPATIAL_UTILS_H
