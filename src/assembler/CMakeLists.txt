cmake_minimum_required(VERSION 3.20)
project(assembler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_definitions(-D_GNU_SOURCE)


# Create the assembler static library
add_library(assembler STATIC
        src/assembler.c
        src/code-generator.c
        src/lexer.c
        src/parser.c
        src/token.c
        src/token_table.c
        src/symbol_table.c
        src/utils.c
)
# Ensure assembler can access its own headers
target_include_directories(assembler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Link common library privately (bundled inside assembler, not exposed)
target_link_libraries(assembler PRIVATE common)


# Define the hackasm executable
add_executable(hackasm src/main.c)
# Link hackasm to assembler (common is handled internally)
target_link_libraries(hackasm PRIVATE assembler common)
# Ensure hackasm has access to assembler headers
target_include_directories(hackasm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Add the tests directory
add_subdirectory(tests)
