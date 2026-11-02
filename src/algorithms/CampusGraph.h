#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>

struct GraphNode {
    std::string id;
    std::string name;
    std::string buildingCode;
    double x; // 2D schematic coordinates (meters)
    double y;
    bool isAccessible;

    GraphNode() : x(0), y(0), isAccessible(true) {}
    GraphNode(std::string id_, std::string name_, std::string code_, double x_, double y_, bool acc_ = true)
        : id(std::move(id_)), name(std::move(name_)), buildingCode(std::move(code_)), x(x_), y(y_), isAccessible(acc_) {}

    double distanceTo(const GraphNode& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

struct WeightedEdge {
    std::string targetNodeId;
    double distanceMeters;
    std::string pathwayType; // "Paved Walkway", "Covered Arcade", "Ramp Access", "Stairs"
    bool wheelchairFriendly;
    std::string directionHint; // "North", "Turn East", etc.

    WeightedEdge(std::string target_, double dist_, std::string pathType_, bool wheel_, std::string hint_)
        : targetNodeId(std::move(target_)), distanceMeters(dist_), pathwayType(std::move(pathType_)),
          wheelchairFriendly(wheel_), directionHint(std::move(hint_)) {}
};

/**
 * @file CampusGraph.h
 * @brief Spatial graph data structure representing campus walkways, buildings, and nodes.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: DATA STRUCTURES & GRAPH ABSTRACTION
 * - Represents the physical campus network using an adjacency list.
 * - Encapsulates nodes and bidirectional weighted edges.
 */
class CampusGraph {
private:
    std::unordered_map<std::string, GraphNode> m_nodes;
    std::unordered_map<std::string, std::vector<WeightedEdge>> m_adjacency;

    void addBidirectionalEdge(const std::string& u, const std::string& v, double dist,
                              const std::string& pathType, bool wheelchair,
                              const std::string& uToVHint, const std::string& vToUHint);

public:
    CampusGraph();
    ~CampusGraph() = default;

    void addNode(const GraphNode& node);
    bool hasNode(const std::string& nodeId) const;
    const GraphNode* getNode(const std::string& nodeId) const;
    const std::vector<WeightedEdge>* getEdges(const std::string& nodeId) const;

    std::vector<std::string> getAllNodeIds() const;
    size_t getNodeCount() const { return m_nodes.size(); }

    void initializeCampusTopology();
};

#endif // CAMPUS_GRAPH_H
