@echo off
title Smart Campus Assistant - C++ OOP & Algorithm Test Suite
color 0A
echo.
echo ================================================================
echo   RUNNING SMART CAMPUS C++ TEST SUITE (OOP & ALGORITHMS)
echo ================================================================
echo.

set "PATH=C:\Qt\Tools\mingw1310_64\bin;C:\MinGW\bin;%PATH%"

echo [1/2] Compiling and running OOP Concepts Verification...
g++ -std=c++17 tests\test_oop_concepts.cpp src\core\CampusEntity.cpp src\core\BuildingEntity.cpp src\core\FacilityEntity.cpp src\core\DepartmentEntity.cpp src\core\FacultyEntity.cpp src\core\NoticeEntity.cpp src\core\FaqEntity.cpp src\core\EntityFactory.cpp src\core\Observer.cpp src\core\SearchStrategy.cpp src\core\CampusDatabase.cpp -o test_oop_concepts.exe
test_oop_concepts.exe

echo.
echo [2/2] Compiling and running Algorithms Verification (Dijkstra, A*, Levenshtein, Trie)...
g++ -std=c++17 tests\test_algorithms.cpp src\algorithms\CampusGraph.cpp src\algorithms\DijkstraRouter.cpp src\algorithms\AStarRouter.cpp src\algorithms\Levenshtein.cpp src\algorithms\Trie.cpp src\algorithms\InvertedIndex.cpp -o test_algorithms.exe
test_algorithms.exe

echo.
echo ================================================================
echo   TEST SUITE EXECUTION COMPLETED
echo ================================================================
pause
