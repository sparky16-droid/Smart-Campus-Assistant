#include "CampusGraph.h"

CampusGraph::CampusGraph() {
    initializeCampusTopology();
}

void CampusGraph::addNode(const GraphNode& node) {
    m_nodes[node.id] = node;
}

bool CampusGraph::hasNode(const std::string& nodeId) const {
    return m_nodes.find(nodeId) != m_nodes.end();
}

const GraphNode* CampusGraph::getNode(const std::string& nodeId) const {
    auto it = m_nodes.find(nodeId);
    if (it != m_nodes.end()) {
        return &(it->second);
    }
    return nullptr;
}

const std::vector<WeightedEdge>* CampusGraph::getEdges(const std::string& nodeId) const {
    auto it = m_adjacency.find(nodeId);
    if (it != m_adjacency.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<std::string> CampusGraph::getAllNodeIds() const {
    std::vector<std::string> ids;
    ids.reserve(m_nodes.size());
    for (const auto& kv : m_nodes) {
        ids.push_back(kv.first);
    }
    return ids;
}

void CampusGraph::addBidirectionalEdge(const std::string& u, const std::string& v, double dist,
                                      const std::string& pathType, bool wheelchair,
                                      const std::string& uToVHint, const std::string& vToUHint)
{
    m_adjacency[u].emplace_back(v, dist, pathType, wheelchair, uToVHint);
    m_adjacency[v].emplace_back(u, dist, pathType, wheelchair, vToUHint);
}

void CampusGraph::initializeCampusTopology() {
    // Campus Nodes with coordinates (x, y in schematic meters)
    addNode(GraphNode("GATE_MAIN", "Campus Main Security Gate", "GATE", 10.0, 10.0, true));
    addNode(GraphNode("NODE_ADM", "Administrative Block", "ADM", 80.0, 70.0, true));
    addNode(GraphNode("NODE_LIB", "Central Library", "LIB", 100.0, 210.0, true));
    addNode(GraphNode("NODE_ENG_A", "Academic Block A - Engineering", "ENG-A", 280.0, 60.0, true));
    addNode(GraphNode("NODE_ENG_B", "Academic Block B - Sciences", "ENG-B", 520.0, 60.0, true));
    addNode(GraphNode("NODE_SAC", "Student Activity Center (SAC)", "SAC", 300.0, 220.0, true));
    addNode(GraphNode("NODE_SPT", "Indoor Sports Complex & Gym", "SPT", 520.0, 220.0, true));
    addNode(GraphNode("NODE_HST", "Hostel Residential Complex", "HST", 720.0, 140.0, true));
    addNode(GraphNode("NODE_MED", "Health Center & Clinic", "MED", 40.0, 360.0, true));
    addNode(GraphNode("NODE_CENTRAL_PLAZA", "Central Fountain Plaza", "PLAZA", 200.0, 140.0, true));

    // Connect nodes with realistic campus pathways and accessibility flags
    addBidirectionalEdge("GATE_MAIN", "NODE_ADM", 85.0, "Tree-lined Avenue", true,
                         "Walk Northeast along the grand palm avenue toward Admin Block",
                         "Walk Southwest along the avenue toward Main Gate");

    addBidirectionalEdge("NODE_ADM", "NODE_CENTRAL_PLAZA", 135.0, "Paved Brick Pathway", true,
                         "Head East past the Dean's quadrangle toward Central Plaza",
                         "Head West toward Admin Block entrance");

    addBidirectionalEdge("NODE_ADM", "NODE_LIB", 140.0, "Covered Arcade Walkway", true,
                         "Walk South along the covered shaded corridor to Central Library",
                         "Walk North along the arcade to Administrative Block");

    addBidirectionalEdge("NODE_LIB", "NODE_CENTRAL_PLAZA", 120.0, "Paved Garden Path", true,
                         "Head Northeast through the reading garden toward Central Plaza",
                         "Head Southwest toward Central Library entrance");

    addBidirectionalEdge("NODE_CENTRAL_PLAZA", "NODE_ENG_A", 110.0, "Granite Promenade", true,
                         "Head Northeast past the fountain toward Engineering Block A",
                         "Head Southwest back to Central Plaza");

    addBidirectionalEdge("NODE_CENTRAL_PLAZA", "NODE_SAC", 130.0, "Broad Pedestrian Walkway", true,
                         "Head Southeast toward Student Activity Center & Food Court",
                         "Head Northwest toward Central Plaza");

    addBidirectionalEdge("NODE_ENG_A", "NODE_ENG_B", 240.0, "Skybridge & Ground Walkway", true,
                         "Walk East along the landscaped engineering corridor to Block B",
                         "Walk West along the corridor back to Block A");

    addBidirectionalEdge("NODE_ENG_A", "NODE_SAC", 160.0, "Courtyard Path", true,
                         "Walk South across the department lawn to SAC Food Court",
                         "Walk North toward Engineering Block A entrance");

    addBidirectionalEdge("NODE_SAC", "NODE_SPT", 220.0, "Sports Boulevard", true,
                         "Walk East past the open amphitheater to Indoor Sports Arena",
                         "Walk West toward Student Activity Center");

    addBidirectionalEdge("NODE_ENG_B", "NODE_SPT", 160.0, "Paved Walkway", true,
                         "Walk South from Block B workshop bays to Sports Arena",
                         "Walk North toward Academic Block B");

    addBidirectionalEdge("NODE_ENG_B", "NODE_HST", 210.0, "Hostel Access Road", true,
                         "Head East along the campus perimeter to Hostel Complex",
                         "Head West toward Academic Block B");

    addBidirectionalEdge("NODE_SPT", "NODE_HST", 215.0, "Athletic Track Path", false,
                         "Head Northeast across the football stadium steps to Hostels",
                         "Head Southwest toward Sports Complex");

    addBidirectionalEdge("NODE_LIB", "NODE_MED", 160.0, "Direct Medical Ramp", true,
                         "Head South toward the Health Care Center & Clinic",
                         "Head North toward Central Library");

    addBidirectionalEdge("GATE_MAIN", "NODE_MED", 350.0, "Perimeter Security Road", true,
                         "Head South along the west campus boundary to Health Clinic",
                         "Head North toward Main Gate");
}
