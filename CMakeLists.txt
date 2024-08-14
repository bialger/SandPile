cmake_minimum_required(VERSION 3.12)

project(
        SandPile
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Labwork3 - SandPile by bialger"
        LANGUAGES C CXX
)

set(CMAKE_CXX_COMPILER_VERSION 20)
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
            /Wall
    )
else ()
    add_compile_options(
            -Werror

            -Wall
            -Wextra
            -Wpedantic

            -Wcast-align
            -Wcast-qual
            -Wconversion
            -Wctor-dtor-privacy
            -Wenum-compare
            -Wfloat-equal
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wredundant-decls
            -Wsign-conversion
            -Wsign-promo
    )

endif ()

add_subdirectory(lib)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} lib_basic
        lib_bmp_writer
        lib_field
        lib_parser
        lib_sand_pile
)
