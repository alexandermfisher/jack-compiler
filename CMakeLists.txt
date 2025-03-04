cmake_minimum_required(VERSION 3.20)
project(jack-compiler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Default to Release if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Compiler flags for different build types
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_MEMCHECK "-g -O0 -fsanitize=address -DDEBUG")

# Add subdirectories for libraries and modules
add_subdirectory(src/common)
add_subdirectory(src/assembler)

# Add tests
enable_testing()
add_subdirectory(tests/assembler)
add_subdirectory(tests/common)
