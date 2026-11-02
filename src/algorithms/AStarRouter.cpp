#include "AStarRouter.h"
#include <queue>
#include <limits>
#include <unordered_map>
#include <algorithm>

AStarRouter::AStarRouter(const CampusGraph& graph)
    : m_graph(graph)
{
}

double AStarRouter::heuristic(const std::string& currentId, const std::string& destinationId) const {
    const auto* u = m_graph.getNode(currentId);
    const auto* v = m_graph.getNode(destinationId);
    if (!u || !v) return 0.0;
    return u->distanceTo(*v);
}

NavigationResult AStarRouter::findPathAStar(const std::string& startId, const std::string& destinationId,
                                            bool wheelchairAccessibleOnly) const
{
    NavigationResult result;
    result.found = false;
    result.startNodeId = startId;
    result.destinationNodeId = destinationId;
    result.totalDistanceMeters = 0.0;
    result.estimatedTimeMinutes = 0.0;

    if (!m_graph.hasNode(startId) || !m_graph.hasNode(destinationId)) {
        return result;
    }

    if (startId == destinationId) {
        result.found = true;
        result.pathNodeIds.push_back(startId);
        return result;
    }

    // Min-heap entry: (f-score = g + h, nodeId)
    typedef std::pair<double, std::string> PriorityPair;
    std::priority_queue<PriorityPair, std::vector<PriorityPair>, std::greater<PriorityPair>> openSet;

    std::unordered_map<std::string, double> gScore;
    std::unordered_map<std::string, double> fScore;
    std::unordered_map<std::string, std::string> cameFrom;
    std::unordered_map<std::string, WeightedEdge> cameViaEdge;

    for (const auto& nid : m_graph.getAllNodeIds()) {
        gScore[nid] = std::numeric_limits<double>::infinity();
        fScore[nid] = std::numeric_limits<double>::infinity();
    }

    gScore[startId] = 0.0;
    fScore[startId] = heuristic(startId, destinationId);
    openSet.push({fScore[startId], startId});

    while (!openSet.empty()) {
        auto top = openSet.top();
        openSet.pop();
        std::string current = top.second;

        if (current == destinationId) {
            break; // Target reached
        }

        const auto* edges = m_graph.getEdges(current);
        if (!edges) continue;

        for (const auto& edge : *edges) {
            if (wheelchairAccessibleOnly && !edge.wheelchairFriendly) {
                continue;
            }

            double tentativeG = gScore[current] + edge.distanceMeters;
            if (tentativeG < gScore[edge.targetNodeId]) {
                cameFrom[edge.targetNodeId] = current;
                cameViaEdge.insert_or_assign(edge.targetNodeId, edge);
                gScore[edge.targetNodeId] = tentativeG;
                fScore[edge.targetNodeId] = tentativeG + heuristic(edge.targetNodeId, destinationId);
                openSet.push({fScore[edge.targetNodeId], edge.targetNodeId});
            }
        }
    }

    if (gScore[destinationId] == std::numeric_limits<double>::infinity()) {
        return result;
    }

    // Reconstruct path
    std::vector<std::string> path;
    std::string curr = destinationId;
    while (curr != startId) {
        path.push_back(curr);
        auto it = cameFrom.find(curr);
        if (it == cameFrom.end()) break;
        curr = it->second;
    }
    path.push_back(startId);
    std::reverse(path.begin(), path.end());

    result.found = true;
    result.pathNodeIds = path;
    result.totalDistanceMeters = gScore[destinationId];
    result.estimatedTimeMinutes = result.totalDistanceMeters / (1.2 * 60.0);

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        std::string from = path[i];
        std::string to = path[i + 1];
        auto it = cameViaEdge.find(to);
        if (it != cameViaEdge.end()) {
            const auto& edge = it->second;
            const auto* toNode = m_graph.getNode(to);
            RouteStep step;
            step.fromNodeId = from;
            step.toNodeId = to;
            step.toNodeName = toNode ? toNode->name : to;
            step.segmentDistanceMeters = edge.distanceMeters;
            step.pathwayType = edge.pathwayType;
            step.instruction = edge.directionHint;
            result.steps.push_back(step);
        }
    }

    return result;
}
