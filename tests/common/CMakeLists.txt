add_executable(test_common test_file_utils.c)

target_link_libraries(test_common PRIVATE common)

add_test(NAME TestCommon COMMAND test_common)
set_tests_properties(TestCommon PROPERTIES LABELS "common")