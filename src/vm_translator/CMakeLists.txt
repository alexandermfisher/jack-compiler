cmake_minimum_required(VERSION 3.20)
project(vm_translator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_definitions(-D_GNU_SOURCE)


# Create the vm_translator static library
add_library(vm_translator STATIC
        # Add your vm_translator sources here
)
target_include_directories(vm_translator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(vm_translator PUBLIC common)


# Define the vmtrans executable
add_executable(vmtrans src/main.c)
target_link_libraries(vmtrans PRIVATE vm_translator)


# Add the tests directory
add_subdirectory(tests)