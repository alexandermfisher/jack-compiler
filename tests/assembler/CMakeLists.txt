# Create test executable
add_executable(
        test_assembler
        test_main.c
        test_parser.c
        test_code.c
        test_symbol_table.c)

target_link_libraries(test_assembler PRIVATE assembler)

# Register the unit test executable as a single test
add_test(NAME UnitTests_Assembler COMMAND test_assembler)
set_tests_properties(UnitTests_Assembler PROPERTIES LABELS "assembler")

# Register the script-based assembler tests separately
add_test(NAME AssemblerTest_All COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_assembler.sh)
set_tests_properties(AssemblerTest_All PROPERTIES LABELS "assembler")

add_test(NAME AssemblerTest_NoSymbols COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_assembler.sh --no-symbols)
set_tests_properties(AssemblerTest_NoSymbols PROPERTIES LABELS "assembler")