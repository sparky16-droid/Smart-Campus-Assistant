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
    src/core/CampusEntity.cpp
    src/core/BuildingEntity.cpp
    src/core/FacilityEntity.cpp
    src/core/DepartmentEntity.cpp
    src/core/FacultyEntity.cpp
    src/core/NoticeEntity.cpp
    src/core/FaqEntity.cpp
    src/core/EntityFactory.cpp
    src/core/Observer.cpp
    src/core/SearchStrategy.cpp
    src/core/CampusDatabase.cpp
    src/algorithms/CampusGraph.cpp
    src/algorithms/DijkstraRouter.cpp
    src/algorithms/AStarRouter.cpp
    src/algorithms/Levenshtein.cpp
    src/algorithms/Trie.cpp
    src/algorithms/InvertedIndex.cpp
    src/nlp/Tokenizer.cpp
    src/nlp/SynonymGraph.cpp
    src/nlp/IntentClassifier.cpp
    src/cli/CampusCLI.cpp
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
    src/core/ICampusEntity.h
    src/core/CampusEntity.h
    src/core/BuildingEntity.h
    src/core/FacilityEntity.h
    src/core/DepartmentEntity.h
    src/core/FacultyEntity.h
    src/core/NoticeEntity.h
    src/core/FaqEntity.h
    src/core/CampusRepository.h
    src/core/EntityFactory.h
    src/core/Observer.h
    src/core/SearchStrategy.h
    src/core/CampusDatabase.h
    src/algorithms/CampusGraph.h
    src/algorithms/DijkstraRouter.h
    src/algorithms/AStarRouter.h
    src/algorithms/Levenshtein.h
    src/algorithms/Trie.h
    src/algorithms/InvertedIndex.h
    src/nlp/Tokenizer.h
    src/nlp/SynonymGraph.h
    src/nlp/IntentClassifier.h
    src/cli/CampusCLI.h
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
