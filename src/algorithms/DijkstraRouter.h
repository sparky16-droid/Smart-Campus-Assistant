#ifndef DIJKSTRA_ROUTER_H
#define DIJKSTRA_ROUTER_H

#include "CampusGraph.h"
#include <vector>
#include <string>

struct RouteStep {
    std::string fromNodeId;
    std::string toNodeId;
    std::string toNodeName;
    double segmentDistanceMeters;
    std::string pathwayType;
    std::string instruction;
};

struct NavigationResult {
    bool found;
    std::string startNodeId;
    std::string destinationNodeId;
    double totalDistanceMeters;
    double estimatedTimeMinutes; // Walking speed approx 1.2 m/s
    std::vector<std::string> pathNodeIds;
    std::vector<RouteStep> steps;

    std::string getFormattedRoute() const;
};

/**
 * @file DijkstraRouter.h
 * @brief Shortest path navigation engine utilizing Dijkstra's algorithm.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS: ALGORITHMS & GRAPH TRAVERSAL
 * - Implements Dijkstra's algorithm using priority queue / min-heap.
 * - Computes minimal walking distance and step-by-step navigation instructions.
 */
class DijkstraRouter {
private:
    const CampusGraph& m_graph;

public:
    explicit DijkstraRouter(const CampusGraph& graph);
    ~DijkstraRouter() = default;

    NavigationResult findShortestPath(const std::string& startId, const std::string& destinationId,
                                      bool wheelchairAccessibleOnly = false) const;
};

#endif // DIJKSTRA_ROUTER_H
