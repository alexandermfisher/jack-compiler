cmake_minimum_required(VERSION 3.20)
project(common_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# List of test sources
set(TEST_SOURCES
        test_file_utils.c
)

foreach(test_file ${TEST_SOURCES})
    # Extract filename without extension (e.g., test_file_utils from test_file_utils.c)
    get_filename_component(test_name ${test_file} NAME_WE)

    # Define test executable
    add_executable(${test_name} ${test_file})

    # Link test executable to common library
    target_link_libraries(${test_name} PRIVATE common)

    # Include common sources privately (only for tests)
    target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/src/common/include)
endforeach()
