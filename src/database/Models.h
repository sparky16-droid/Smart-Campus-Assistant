#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QStringList>
#include <QDateTime>

struct Building {
    QString id;
    QString name;
    QString code;
    QString description;
    double latitude;
    double longitude;
    int floors;
    int yearBuilt;
    bool wheelchairAccessible;
    int mapX; // Visual schematic coordinates
    int mapY;
    int mapWidth;
    int mapHeight;
    QString color;
};

struct Facility {
    QString id;
    QString name;
    QString category; // Academic, Library, Cafeteria, Sports, Lab, Hostel, Administration, Medical, Transport, ATM
    QString description;
    QString buildingId;
    QString buildingName;
    QString floor;
    QString roomNumber;
    QString landmark;
    QString timings;
    QString contactPhone;
    QString contactEmail;
    bool isOperational;
};

struct Department {
    QString id;
    QString name;
    QString code;
    QString description;
    QString buildingId;
    QString buildingName;
    QString headOfDepartment;
    QString contactEmail;
    QString contactPhone;
    QString websiteUrl;
    QString programsOffered;
};

struct Faculty {
    QString id;
    QString name;
    QString title; // Dr., Prof., etc.
    QString designation;
    QString departmentId;
    QString departmentName;
    QString email;
    QString phone;
    QString cabinRoom;
    QString officeHours;
    QString specialization;
    QString bio;
};

struct Notice {
    QString id;
    QString title;
    QString content;
    QString category; // Academic, Event, Exam, Placement, General
    QString priority; // Normal, High, Urgent
    QDateTime publishDate;
    bool isPublished;
    QString author;
};

struct Faq {
    QString id;
    QString question;
    QString answer;
    QString category;
    QString keywords;
    int viewCount;
};

struct Synonym {
    QString id;
    QString primaryWord;
    QStringList synonyms;
};

struct ChatMessageRecord {
    QString id;
    QString sessionId;
    QString role; // "user" or "assistant"
    QString content;
    bool helpful; // true = helpful, false = unhelpful
    bool hasFeedback;
    QString referencedEntity;
    QDateTime createdAt;
};

struct ChatReference {
    QString type; // "facility", "department", "faculty", "notice", "building", "faq"
    QString id;
    QString name;
    QString buildingName;
    QString floor;
    QString roomNumber;
    QString timings;
    QString linkUrl;
};

#endif // MODELS_H
