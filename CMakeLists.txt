cmake_minimum_required(VERSION 3.17)
project(ChessHistoryFinder)

set(CMAKE_CXX_STANDARD 11)
set(SFML_STATIC_LIBRARIES true)
find_package(SFML COMPONENTS graphics window system REQUIRED HINTS "SFML/lib/cmake/SFML")

file(GLOB ChessHistoryFinder "*.cpp" "*.h")
add_executable(ChessHistoryFinder ${ChessHistoryFinder})
target_link_libraries(ChessHistoryFinder sfml-graphics sfml-window sfml-system)