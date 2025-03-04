cmake_minimum_required(VERSION 3.20)
project(assembler)

# Define the assembler library
add_library(assembler STATIC
        assembler.c
        code.c
        parser.c
        symbol_table.c
)

# Include common headers
target_include_directories(assembler PUBLIC ../common)

# Define assembler executable (links against assembler library)
add_executable(hackasm main.c)
target_link_libraries(hackasm PRIVATE assembler common)

# Include common headers
target_include_directories(hackasm PRIVATE ../common)

# Link assembler with common library
target_link_libraries(hackasm PRIVATE common)