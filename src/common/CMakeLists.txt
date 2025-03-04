cmake_minimum_required(VERSION 3.20)
project(common)

add_library(common STATIC
        file_utils.c
)

# Ensure common can access shared headers
target_include_directories(common PUBLIC
        ${CMAKE_SOURCE_DIR}/src/include  # Shared headers
)
