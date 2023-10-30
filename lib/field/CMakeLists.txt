add_library(lib_field STATIC
        coordinates_field.cpp
        coordinates_field.hpp
        queue.hpp
        queue.cpp
        tsv_handler.cpp
        tsv_handler.hpp
)

target_link_libraries(lib_field PUBLIC lib_basic)

target_include_directories(lib_field PUBLIC ${PROJECT_SOURCE_DIR})