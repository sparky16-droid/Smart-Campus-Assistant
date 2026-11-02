#include "IntentClassifier.h"
#include <algorithm>
#include <cctype>

static std::string toLowerIntent(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string IntentClassifier::intentToString(UserIntent intent) {
    switch (intent) {
        case UserIntent::Greeting:         return "Greeting";
        case UserIntent::Navigation:       return "Navigation";
        case UserIntent::FacilitySearch:   return "FacilitySearch";
        case UserIntent::FacultySearch:    return "FacultySearch";
        case UserIntent::DepartmentSearch: return "DepartmentSearch";
        case UserIntent::NoticeSearch:     return "NoticeSearch";
        case UserIntent::FaqSearch:        return "FaqSearch";
        case UserIntent::AdminAction:      return "AdminAction";
        default:                           return "GeneralQuery";
    }
}

IntentClassificationResult IntentClassifier::classify(const std::string& userQuery) const {
    IntentClassificationResult res;
    res.intent = UserIntent::GeneralQuery;
    res.confidence = 0.50;

    std::string q = toLowerIntent(userQuery);

    // 1. Greetings
    if (q == "hi" || q == "hello" || q == "hey" || q == "good morning" || q == "good afternoon" || q == "namaste") {
        res.intent = UserIntent::Greeting;
        res.confidence = 0.99;
        res.intentName = intentToString(res.intent);
        return res;
    }

    // 2. Navigation / Directions
    if (q.find("how to reach") != std::string::npos ||
        q.find("where is") != std::string::npos ||
        q.find("route") != std::string::npos ||
        q.find("direction") != std::string::npos ||
        q.find("path to") != std::string::npos ||
        q.find("navigate to") != std::string::npos ||
        q.find("shortest path") != std::string::npos)
    {
        res.intent = UserIntent::Navigation;
        res.confidence = 0.92;
        res.intentName = intentToString(res.intent);
        return res;
    }

    // 3. Faculty / Professor Search
    if (q.find("professor") != std::string::npos ||
        q.find("faculty") != std::string::npos ||
        q.find("teacher") != std::string::npos ||
        q.find("hod") != std::string::npos ||
        q.find("cabin") != std::string::npos ||
        q.find("office hours") != std::string::npos ||
        q.find("dr.") != std::string::npos)
    {
        res.intent = UserIntent::FacultySearch;
        res.confidence = 0.88;
        res.intentName = intentToString(res.intent);
        return res;
    }

    // 4. Notices / Circulars
    if (q.find("notice") != std::string::npos ||
        q.find("circular") != std::string::npos ||
        q.find("announcement") != std::string::npos ||
        q.find("exam schedule") != std::string::npos ||
        q.find("timetable") != std::string::npos ||
        q.find("deadline") != std::string::npos)
    {
        res.intent = UserIntent::NoticeSearch;
        res.confidence = 0.85;
        res.intentName = intentToString(res.intent);
        return res;
    }

    // 5. Facility Search
    if (q.find("library") != std::string::npos ||
        q.find("canteen") != std::string::npos ||
        q.find("cafeteria") != std::string::npos ||
        q.find("gym") != std::string::npos ||
        q.find("sports") != std::string::npos ||
        q.find("clinic") != std::string::npos ||
        q.find("doctor") != std::string::npos ||
        q.find("atm") != std::string::npos ||
        q.find("lab") != std::string::npos ||
        q.find("store") != std::string::npos ||
        q.find("stationery") != std::string::npos)
    {
        res.intent = UserIntent::FacilitySearch;
        res.confidence = 0.82;
        res.intentName = intentToString(res.intent);
        return res;
    }

    // 6. Department Search
    if (q.find("department") != std::string::npos ||
        q.find("branch") != std::string::npos ||
        q.find("engineering") != std::string::npos ||
        q.find("course") != std::string::npos ||
        q.find("curriculum") != std::string::npos ||
        q.find("b.tech") != std::string::npos)
    {
        res.intent = UserIntent::DepartmentSearch;
        res.confidence = 0.80;
        res.intentName = intentToString(res.intent);
        return res;
    }

    // 7. General FAQs
    if (q.find("how do i") != std::string::npos ||
        q.find("what is") != std::string::npos ||
        q.find("wifi") != std::string::npos ||
        q.find("attendance") != std::string::npos ||
        q.find("fee") != std::string::npos ||
        q.find("hostel") != std::string::npos)
    {
        res.intent = UserIntent::FaqSearch;
        res.confidence = 0.78;
        res.intentName = intentToString(res.intent);
        return res;
    }

    res.intentName = intentToString(res.intent);
    return res;
}
