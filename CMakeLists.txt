project(ChessHistoryFinder)

set(CMAKE_CXX_STANDARD 17)
set(SFML_STATIC_LIBRARIES true)
find_package(SFML COMPONENTS graphics window system REQUIRED)

file(GLOB ChessHistoryFinder "*.cpp" "*.h")
add_executable(ChessHistoryFinder ${ChessHistoryFinder})
target_link_libraries(ChessHistoryFinder sfml-graphics sfml-window sfml-system)