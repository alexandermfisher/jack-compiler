cmake_minimum_required(VERSION 3.20)
project(jack-compiler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Default to Release if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type (Debug, Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Detect platform
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(APPLE_BUILD TRUE)
else()
    set(APPLE_BUILD FALSE)
endif()

# General warning flags
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")

# Debug flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG -fsanitize=undefined,address -fno-omit-frame-pointer -fstack-protector-strong")

# Release flags
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -march=native -flto")

# MemCheck option (enabled via -DENABLE_MEMCHECK=ON)
option(ENABLE_MEMCHECK "Enable memory checking tools" OFF)
if(ENABLE_MEMCHECK)
    message(STATUS "Enabling MemCheck flags")
    if(APPLE_BUILD)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined,leak")
    endif()
endif()

# Apply the correct flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")
endif()

# Print the final compiler flags
message(STATUS "C Compiler Flags: ${CMAKE_C_FLAGS}")

# Add subdirectories for libraries and modules
add_subdirectory(src/common)
add_subdirectory(src/assembler)
add_subdirectory(tests/assembler)
add_subdirectory(tests/common)

