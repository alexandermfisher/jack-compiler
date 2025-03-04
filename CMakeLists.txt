cmake_minimum_required(VERSION 3.20)
project(jack-compiler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src/common)    # Build common library
add_subdirectory(src/assembler) # Build assembler


## Add tests
enable_testing()
add_subdirectory(tests)

