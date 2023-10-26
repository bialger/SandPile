cmake_minimum_required(VERSION 3.12)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_subdirectory(basic)
add_subdirectory(bmp_writer)
add_subdirectory(field)
add_subdirectory(parser)
add_subdirectory(sand_pile)
