add_library(lib_bmp_writer STATIC bmp_writer.cpp bmp_writer.hpp)

target_include_directories(lib_bmp_writer PUBLIC ${PROJECT_SOURCE_DIR})
