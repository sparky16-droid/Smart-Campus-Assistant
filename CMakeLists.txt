cmake_minimum_required(VERSION 3.16)
project(SmartCampusAssistant VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql Network)

set(SOURCES
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/nlp/SynonymEngine.cpp
    src/nlp/ChatbotEngine.cpp
    src/ui/MainWindow.cpp
    src/ui/CampusMapWidget.cpp
    src/ui/ChatbotWidget.cpp
    src/ui/HomeWidget.cpp
    src/ui/DirectoryWidgets.cpp
    src/ui/NoticesFaqWidgets.cpp
    src/ui/AdminDialog.cpp
    src/server/LocalWebServer.cpp
)

set(HEADERS
    src/database/Models.h
    src/database/DatabaseManager.h
    src/nlp/SynonymEngine.h
    src/nlp/ChatbotEngine.h
    src/ui/ThemeHelper.h
    src/ui/MainWindow.h
    src/ui/CampusMapWidget.h
    src/ui/ChatbotWidget.h
    src/ui/HomeWidget.h
    src/ui/DirectoryWidgets.h
    src/ui/NoticesFaqWidgets.h
    src/ui/AdminDialog.h
    src/server/LocalWebServer.h
)

add_executable(SmartCampusAssistant ${SOURCES} ${HEADERS})

target_link_libraries(SmartCampusAssistant PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
)

target_include_directories(SmartCampusAssistant PRIVATE
    src
    src/database
    src/nlp
    src/ui
    src/server
)

# Automated Test Target
add_executable(test_campus
    tests/test_campus.cpp
    src/database/DatabaseManager.cpp
    src/nlp/SynonymEngine.cpp
    src/nlp/ChatbotEngine.cpp
)

target_link_libraries(test_campus PRIVATE
    Qt6::Core
    Qt6::Sql
)

target_include_directories(test_campus PRIVATE
    src
    src/database
    src/nlp
)
