cmake_minimum_required(VERSION 3.20)
project(common)

# Create a static library from common sources
add_library(common STATIC
        file_utils.c
)

# Allow other targets to include common headers
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})