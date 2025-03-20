cmake_minimum_required(VERSION 3.20)
project(common C)

# Create the common static library
add_library(common STATIC
        src/file_utils.c
        src/token_table.c
)

# Ensure common provides its headers to any dependent target
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add the tests directory (if it exists)
add_subdirectory(tests)
