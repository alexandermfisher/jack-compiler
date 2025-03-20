cmake_minimum_required(VERSION 3.20)
project(assembler C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_definitions(-D_GNU_SOURCE)


# Create the assembler static library
add_library(assembler STATIC
        src/assembler.c
        src/code_generator.c
        src/parser.c
        src/lexer.c
        src/token.c
        src/symbol_table.c
)
# Ensure assembler can access its own headers
target_include_directories(assembler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Link common library publicly
target_link_libraries(assembler PUBLIC common)


# Define the hackasm executable
add_executable(hackasm src/main.c)
target_link_libraries(hackasm PRIVATE assembler)


# Add the tests directory
add_subdirectory(tests)
