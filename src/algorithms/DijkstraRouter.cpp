#include "DijkstraRouter.h"
#include <queue>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <iomanip>

std::string NavigationResult::getFormattedRoute() const {
    if (!found) {
        return "No valid pedestrian route found between the specified campus locations.";
    }
    std::stringstream ss;
    ss << "============================================================\n"
       << " CAMPUS NAVIGATION: " << startNodeId << " -> " << destinationNodeId << "\n"
       << " Total Walking Distance: " << std::fixed << std::setprecision(1) << totalDistanceMeters << " meters\n"
       << " Estimated Walking Time: ~" << std::setprecision(1) << estimatedTimeMinutes << " minutes (at 1.2 m/s)\n"
       << " Route (" << pathNodeIds.size() << " waypoints): ";
    for (size_t i = 0; i < pathNodeIds.size(); ++i) {
        ss << pathNodeIds[i] << (i + 1 < pathNodeIds.size() ? " -> " : "\n");
    }
    ss << "============================================================\n"
       << "STEP-BY-STEP DIRECTIONS:\n";
    for (size_t i = 0; i < steps.size(); ++i) {
        ss << " " << (i + 1) << ". " << steps[i].instruction
           << " [" << std::fixed << std::setprecision(0) << steps[i].segmentDistanceMeters 
           << "m via " << steps[i].pathwayType << "]\n";
    }
    ss << " Arrived at destination: " << destinationNodeId << "!\n";
    return ss.str();
}

DijkstraRouter::DijkstraRouter(const CampusGraph& graph)
    : m_graph(graph)
{
}

NavigationResult DijkstraRouter::findShortestPath(const std::string& startId, const std::string& destinationId,
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

    // Min-heap entry: (current shortest distance, nodeId)
    typedef std::pair<double, std::string> DistNodePair;
    std::priority_queue<DistNodePair, std::vector<DistNodePair>, std::greater<DistNodePair>> pq;

    std::unordered_map<std::string, double> distances;
    std::unordered_map<std::string, std::string> previousNode;
    std::unordered_map<std::string, WeightedEdge> previousEdge;

    for (const auto& nid : m_graph.getAllNodeIds()) {
        distances[nid] = std::numeric_limits<double>::infinity();
    }

    distances[startId] = 0.0;
    pq.push({0.0, startId});

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        double currentDist = top.first;
        std::string u = top.second;

        if (currentDist > distances[u]) {
            continue;
        }

        if (u == destinationId) {
            break; // Target reached
        }

        const auto* edges = m_graph.getEdges(u);
        if (!edges) continue;

        for (const auto& edge : *edges) {
            if (wheelchairAccessibleOnly && !edge.wheelchairFriendly) {
                continue; // Skip inaccessible route
            }

            double alt = currentDist + edge.distanceMeters;
            if (alt < distances[edge.targetNodeId]) {
                distances[edge.targetNodeId] = alt;
                previousNode[edge.targetNodeId] = u;
                previousEdge.insert_or_assign(edge.targetNodeId, edge);
                pq.push({alt, edge.targetNodeId});
            }
        }
    }

    if (distances[destinationId] == std::numeric_limits<double>::infinity()) {
        return result; // Unreachable
    }

    // Reconstruct path backwards from destination
    std::vector<std::string> path;
    std::string curr = destinationId;
    while (curr != startId) {
        path.push_back(curr);
        auto it = previousNode.find(curr);
        if (it == previousNode.end()) break;
        curr = it->second;
    }
    path.push_back(startId);
    std::reverse(path.begin(), path.end());

    result.found = true;
    result.pathNodeIds = path;
    result.totalDistanceMeters = distances[destinationId];
    result.estimatedTimeMinutes = result.totalDistanceMeters / (1.2 * 60.0); // 1.2 m/s avg walking speed

    // Generate step-by-step navigation instructions
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        std::string from = path[i];
        std::string to = path[i + 1];
        auto it = previousEdge.find(to);
        if (it != previousEdge.end()) {
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
