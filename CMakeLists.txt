cmake_minimum_required(VERSION 3.20)
project(jack-compiler C)

# Default to Release if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type (Debug, Release)" FORCE)
endif()

# General warning flags
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")

# Debug flags (no AddressSanitizer by default)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG -fno-omit-frame-pointer -fstack-protector-strong")

# Release flags
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -march=native -flto")

# MemCheck option (enabled via -DENABLE_MEMCHECK=ON)
option(ENABLE_MEMCHECK "Enable memory checking tools (Valgrind Memcheck)" OFF)

# Memcheck flags only if ENABLE_MEMCHECK is ON
if(ENABLE_MEMCHECK)
    message(STATUS "Enabling MemCheck flags (Valgrind)")
    set(CMAKE_C_FLAGS_MEMCHECK "-fno-omit-frame-pointer -fstack-protector-strong")  # Memcheck-specific flags without AddressSanitizer
else()
    message(STATUS "Enabling AddressSanitizer (ASan) flags")
    set(CMAKE_C_FLAGS_ASAN "-fsanitize=address,undefined")  # AddressSanitizer flags for Debug
endif()

# Apply the correct flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}")
    if(NOT ENABLE_MEMCHECK)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_ASAN}")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")
endif()

# Add Memcheck flags if ENABLE_MEMCHECK is ON
if(ENABLE_MEMCHECK)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_MEMCHECK}")
endif()

# Print the final compiler flags
message(STATUS "C Compiler Flags: ${CMAKE_C_FLAGS}")

# Add subdirectories for common and assembler components
add_subdirectory(src/common)
add_subdirectory(src/assembler)