add_library(lib_basic STATIC basic_functions.cpp basic_functions.hpp)

target_include_directories(lib_basic PUBLIC ${PROJECT_SOURCE_DIR})
