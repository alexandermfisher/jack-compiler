cmake_minimum_required(VERSION 3.20)
project(assembler)

add_library(assembler STATIC
        assembler.c
        code.c
        parser.c
        symbol_table.c
)

# Ensure assembler can access shared headers
target_include_directories(assembler PUBLIC
        ${CMAKE_SOURCE_DIR}/src/include  # Shared headers
)

# Link assembler with common if it depends on file_utils.c
target_link_libraries(assembler PRIVATE common)

# Define assembler executable
add_executable(hackasm main.c)
target_link_libraries(hackasm PRIVATE assembler common)

# Ensure hackasm can access shared headers
target_include_directories(hackasm PRIVATE
        ${CMAKE_SOURCE_DIR}/src/include
)
