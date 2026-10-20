QT += core gui widgets sql network

CONFIG += c++17

TARGET = SmartCampusAssistant
TEMPLATE = app

INCLUDEPATH += src \
               src/database \
               src/nlp \
               src/ui \
               src/server

HEADERS += \
    src/database/Models.h \
    src/database/DatabaseManager.h \
    src/nlp/SynonymEngine.h \
    src/nlp/ChatbotEngine.h \
    src/ui/ThemeHelper.h \
    src/ui/MainWindow.h \
    src/ui/CampusMapWidget.h \
    src/ui/ChatbotWidget.h \
    src/ui/HomeWidget.h \
    src/ui/DirectoryWidgets.h \
    src/ui/NoticesFaqWidgets.h \
    src/ui/AdminDialog.h \
    src/server/LocalWebServer.h

SOURCES += \
    src/main.cpp \
    src/database/DatabaseManager.cpp \
    src/nlp/SynonymEngine.cpp \
    src/nlp/ChatbotEngine.cpp \
    src/ui/MainWindow.cpp \
    src/ui/CampusMapWidget.cpp \
    src/ui/ChatbotWidget.cpp \
    src/ui/HomeWidget.cpp \
    src/ui/DirectoryWidgets.cpp \
    src/ui/NoticesFaqWidgets.cpp \
    src/ui/AdminDialog.cpp \
    src/server/LocalWebServer.cpp
