#ifndef ASTAR_ROUTER_H
#define ASTAR_ROUTER_H

#include "CampusGraph.h"
#include "DijkstraRouter.h"
#include <string>

/**
 * @file AStarRouter.h
 * @brief A* heuristic pathfinding algorithm utilizing spatial Euclidean distance.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: HEURISTIC ALGORITHMS & PATHFINDING
 * - Implements f(n) = g(n) + h(n), where h(n) is the straight-line Euclidean distance
 *   to the destination node.
 * - Guarantees optimal pathfinding with fewer node expansions than standard Dijkstra.
 */
class AStarRouter {
private:
    const CampusGraph& m_graph;

    double heuristic(const std::string& currentId, const std::string& destinationId) const;

public:
    explicit AStarRouter(const CampusGraph& graph);
    ~AStarRouter() = default;

    NavigationResult findPathAStar(const std::string& startId, const std::string& destinationId,
                                   bool wheelchairAccessibleOnly = false) const;
};

#endif // ASTAR_ROUTER_H
