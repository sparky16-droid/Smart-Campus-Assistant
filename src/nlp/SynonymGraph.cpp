#include "SynonymGraph.h"
#include <algorithm>
#include <cctype>

SynonymGraph::SynonymGraph() {
    // 1. Food / Cafeteria
    addSynonymGroup("cafeteria", {"canteen", "food", "mess", "lunch", "breakfast", "dinner", "snacks", "cafe", "foodcourt", "eatery"});

    // 2. Library / Books
    addSynonymGroup("library", {"books", "reading", "study", "journal", "periodicals", "reference", "digital library"});

    // 3. Medical / Doctor
    addSynonymGroup("medical", {"doctor", "clinic", "hospital", "dispensary", "emergency", "health", "pharmacy", "medicine", "firstaid"});

    // 4. Sports / Fitness
    addSynonymGroup("sports", {"gym", "gymnasium", "fitness", "workout", "badminton", "tabletennis", "cricket", "football", "arena", "athletics"});

    // 5. Hostels / Accommodation
    addSynonymGroup("hostel", {"dorm", "dormitory", "residence", "hostels", "room", "accommodation", "warden", "mess"});

    // 6. Administration / Officials
    addSynonymGroup("administration", {"office", "registrar", "principal", "dean", "director", "accounts", "fees", "bonafide", "transcripts"});

    // 7. Computer / Labs
    addSynonymGroup("computer", {"computing", "coding", "programming", "software", "hpc", "servers", "workstations", "lab"});

    // 8. Washroom / Restroom
    addSynonymGroup("washroom", {"restroom", "toilet", "lavatory", "loo", "urinal"});

    // 9. ATM / Banking
    addSynonymGroup("atm", {"bank", "banking", "cash", "money", "sbi"});

    // 10. Placements / Careers
    addSynonymGroup("placement", {"jobs", "career", "internship", "companies", "interviews", "recruitment", "training"});
}

static std::string toLowerSyn(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

void SynonymGraph::addSynonymGroup(const std::string& canonical, const std::vector<std::string>& synonyms) {
    std::string canonLower = toLowerSyn(canonical);
    m_canonicalToSynonyms[canonLower] = synonyms;
    m_synonymToCanonical[canonLower] = canonLower;

    for (const auto& syn : synonyms) {
        m_synonymToCanonical[toLowerSyn(syn)] = canonLower;
    }
}

std::string SynonymGraph::getCanonicalTerm(const std::string& word) const {
    std::string w = toLowerSyn(word);
    auto it = m_synonymToCanonical.find(w);
    if (it != m_synonymToCanonical.end()) {
        return it->second;
    }
    return w;
}

std::vector<std::string> SynonymGraph::expandTokens(const std::vector<std::string>& tokens) const {
    std::unordered_set<std::string> expandedSet;
    for (const auto& t : tokens) {
        std::string w = toLowerSyn(t);
        expandedSet.insert(w);

        auto it = m_synonymToCanonical.find(w);
        if (it != m_synonymToCanonical.end()) {
            std::string canonical = it->second;
            expandedSet.insert(canonical);

            auto synIt = m_canonicalToSynonyms.find(canonical);
            if (synIt != m_canonicalToSynonyms.end()) {
                for (const auto& syn : synIt->second) {
                    expandedSet.insert(syn);
                }
            }
        }
    }
    return std::vector<std::string>(expandedSet.begin(), expandedSet.end());
}

bool SynonymGraph::areSynonymous(const std::string& word1, const std::string& word2) const {
    return getCanonicalTerm(word1) == getCanonicalTerm(word2);
}
