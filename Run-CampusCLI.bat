@echo off
title Smart Campus Assistant - C++ Interactive Console
color 0B
echo.
echo ================================================================
echo   LAUNCHING SMART CAMPUS ASSISTANT (C++ OOP STANDALONE CLI)
echo ================================================================
echo.

if not exist CampusAssistantCLI.exe (
    echo [INFO] Compiling CampusAssistantCLI.exe using MinGW GCC 13...
    set "PATH=C:\Qt\Tools\mingw1310_64\bin;C:\MinGW\bin;%PATH%"
    g++ -std=c++17 -O2 src\cli_main.cpp src\core\CampusEntity.cpp src\core\BuildingEntity.cpp src\core\FacilityEntity.cpp src\core\DepartmentEntity.cpp src\core\FacultyEntity.cpp src\core\NoticeEntity.cpp src\core\FaqEntity.cpp src\core\EntityFactory.cpp src\core\Observer.cpp src\core\SearchStrategy.cpp src\core\CampusDatabase.cpp src\algorithms\CampusGraph.cpp src\algorithms\DijkstraRouter.cpp src\algorithms\AStarRouter.cpp src\algorithms\Levenshtein.cpp src\algorithms\Trie.cpp src\algorithms\InvertedIndex.cpp src\nlp\Tokenizer.cpp src\nlp\SynonymGraph.cpp src\nlp\IntentClassifier.cpp src\cli\CampusCLI.cpp -o CampusAssistantCLI.exe
)

CampusAssistantCLI.exe
echo.
pause
