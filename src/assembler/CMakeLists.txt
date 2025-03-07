cmake_minimum_required(VERSION 3.20)
project(assembler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create the assembler static library
add_library(assembler STATIC
        src/assembler.c
        src/code-generator.c
        src/parser.c
        src/symbol_table.c
        src/utils.c
)

# Ensure assembler can access its own headers
target_include_directories(assembler PUBLIC ${CMAKE_SOURCE_DIR}/src/assembler/include)

# Link against the common library
target_link_libraries(assembler PRIVATE common)

# Define the hackasm executable
add_executable(hackasm src/main.c)

# Link hackasm to the assembler library and common library
target_link_libraries(hackasm PRIVATE assembler common)

# Ensure hackasm has access to assembler headers
target_include_directories(hackasm PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler/include)

# Add the tests directory
add_subdirectory(tests)
