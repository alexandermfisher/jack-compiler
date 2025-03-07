cmake_minimum_required(VERSION 3.20)
project(common C)

# Create the common library
add_library(common STATIC
        src/file_utils.c
)

# Include directories for common (so other targets can use its headers)
target_include_directories(common PUBLIC ${CMAKE_SOURCE_DIR}/src/common/include)

# Add the tests directory
add_subdirectory(tests)