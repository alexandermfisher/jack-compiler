cmake_minimum_required(VERSION 3.20)
project(assembler_tests C)

# List of test source files
set(TEST_SOURCES
        test_code_generator.c
        test_parser.c
        test_token.c
        test_token_table.c
        test_symbol_table.c
        test_utils.c
)

# Iterate over each test file and create an executable for it
foreach(test_file ${TEST_SOURCES})
    # Extract the filename without the extension (e.g., test_assembler from test_assembler.c)
    get_filename_component(test_name ${test_file} NAME_WE)

    # Define a test executable for each test file
    add_executable(${test_name} ${test_file})

    # Link the test executable to the assembler and common libraries
    target_link_libraries(${test_name} PRIVATE assembler common)

    # Include the necessary header directories for assembler and common
    target_include_directories(${test_name} PRIVATE
            ${CMAKE_SOURCE_DIR}/src/assembler/include
            ${CMAKE_SOURCE_DIR}/src/assembler/src  # Include assembler private headers
            ${CMAKE_SOURCE_DIR}/src/common/include  # Include common headers
    )
endforeach()
