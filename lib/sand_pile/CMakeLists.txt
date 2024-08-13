add_library(lib_sand_pile STATIC sand_pile.cpp sand_pile.hpp)

target_link_libraries(lib_sand_pile PUBLIC lib_bmp_writer lib_field)

target_include_directories(lib_sand_pile PUBLIC ${PROJECT_SOURCE_DIR})
