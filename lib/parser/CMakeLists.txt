add_library(lib_parser STATIC arguments_parser.cpp arguments_parser.hpp)

target_include_directories(lib_parser PUBLIC ${PROJECT_SOURCE_DIR})
