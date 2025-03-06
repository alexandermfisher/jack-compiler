# Create individual test executables

add_executable(test_assembler test_assembler.c)
target_link_libraries(test_assembler PRIVATE assembler)
target_include_directories(test_assembler PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler)

add_executable(test_parser test_parser.c)
target_link_libraries(test_parser PRIVATE assembler)
target_include_directories(test_parser PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler)

add_executable(test_code test_code.c)
target_link_libraries(test_code PRIVATE assembler)
target_include_directories(test_code PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler)

add_executable(test_symbol_table test_symbol_table.c)
target_link_libraries(test_symbol_table PRIVATE assembler)
target_include_directories(test_symbol_table PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler)

add_executable(test_utils test_utils.c)
target_link_libraries(test_utils PRIVATE assembler)
target_include_directories(test_utils PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler)
