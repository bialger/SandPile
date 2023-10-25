add_library(lib_sand_pile STATIC sand_pile.cpp sand_pile.hpp)

target_include_directories(lib_sand_pile PUBLIC ${PROJECT_SOURCE_DIR})
