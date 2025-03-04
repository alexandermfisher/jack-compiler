# Create test executable for unit tests
add_executable(
        test_assembler
        test_main.c
        test_assembler.c
        test_parser.c
        test_code.c
        test_symbol_table.c
)

# Link the assembler library
target_link_libraries(test_assembler PRIVATE assembler)

# Register unit tests
add_test(NAME UnitTests_Assembler COMMAND test_assembler)
set_tests_properties(UnitTests_Assembler PROPERTIES LABELS "assembler")

# Define the path to the integration test script
set(INTEGRATION_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/integration/test_assembler.sh")

# Ensure the script is executable
add_custom_target(
        MakeTestScriptExecutable
        COMMAND chmod +x ${INTEGRATION_SCRIPT}
)

## Integration tests
#add_test(NAME AssemblerTest_All COMMAND ${INTEGRATION_SCRIPT})
#set_tests_properties(AssemblerTest_All PROPERTIES LABELS "assembler")
#
#add_test(NAME AssemblerTest_NoSymbols COMMAND ${INTEGRATION_SCRIPT} --no-symbols)
#set_tests_properties(AssemblerTest_NoSymbols PROPERTIES LABELS "assembler")
#
#add_test(NAME AssemblerTest_Valgrind COMMAND valgrind --leak-check=full --error-exitcode=1 ${INTEGRATION_SCRIPT})
#set_tests_properties(AssemblerTest_Valgrind PROPERTIES LABELS "assembler" WILL_FAIL FALSE)