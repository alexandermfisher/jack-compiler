cmake_minimum_required(VERSION 3.20)
project(assembler)

# Define assembler executable
add_executable(hackasm
        main.c
        assembler.c
        code.c
        parser.c
        symbol_table.c
)

# Include common headers
target_include_directories(hackasm PRIVATE ../common)

# Link assembler with common library
target_link_libraries(hackasm PRIVATE common)