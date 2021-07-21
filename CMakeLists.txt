cmake_minimum_required(VERSION 3.17)
project(ChessHistoryFinder)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

file(GLOB ChessHistoryFinder "*.cpp" "*.h")
add_executable(ChessHistoryFinder main.cpp)
target_link_libraries(ChessHistoryFinder sfml-graphics sfml-window sfml-system)