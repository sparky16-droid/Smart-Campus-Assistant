#ifndef INTENT_CLASSIFIER_H
#define INTENT_CLASSIFIER_H

#include <string>
#include <vector>

enum class UserIntent {
    Greeting,
    Navigation,
    FacilitySearch,
    FacultySearch,
    DepartmentSearch,
    NoticeSearch,
    FaqSearch,
    AdminAction,
    GeneralQuery
};

struct IntentClassificationResult {
    UserIntent intent;
    std::string intentName;
    double confidence;
    std::string extractedEntity;
    std::vector<std::string> keywords;
};

/**
 * @file IntentClassifier.h
 * @brief Categorizes incoming natural language questions into campus assistant actions.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: INTENT RECOGNITION & PATTERN MATCHING
 * - Identifies whether the user is asking for directions, looking up a professor,
 *   checking notices, or asking general campus FAQs.
 */
class IntentClassifier {
public:
    IntentClassifier() = default;
    ~IntentClassifier() = default;

    IntentClassificationResult classify(const std::string& userQuery) const;
    static std::string intentToString(UserIntent intent);
};

#endif // INTENT_CLASSIFIER_H
