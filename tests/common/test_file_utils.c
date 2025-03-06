//
// Created by Alexander Fisher on 03/03/2025.
//
#include "file_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_has_extension(void);
void test_change_extension(void);


int main(void) {
    printf("Starting test_file_utils...\n");
    test_has_extension();
    test_change_extension();
    printf("All tests passed successfully.\n");
    printf("Test completed.\n");
    return 0;
}

void test_has_extension(void) {
    assert(has_extension("test.asm", ".asm") == true);
    assert(has_extension("test.hack", ".hack") == true);
    assert(has_extension("test", ".asm") == false);
    assert(has_extension(NULL, ".asm") == false);
    assert(has_extension("test.asm", NULL) == false);
    assert(has_extension("file/path/source.asmnot", ".asm") == false);
    assert(has_extension("file/path/source.asm.text", ".asm") == false);
    assert(has_extension("file/path/source.asm.asm", ".asm") == false);
    assert(has_extension("file/../source.asm.asm", ".asm.asm") == true);
    assert(has_extension("file/path/source.text.asm", ".asm") == false);
    assert(has_extension("file/../source.text.asm", ".asm") == false);
    assert(has_extension("file/../source.asm", ".asm") == true);
}

void test_change_extension(void) {
    char output[256];

    // Test replacing an extension
    assert(change_extension("file.txt", ".md", output, sizeof(output)) == true);
    assert(strcmp(output, "file.md") == 0);

    // Test appending an extension when none exists
    assert(change_extension("file", ".md", output, sizeof(output)) == true);
    assert(strcmp(output, "file.md") == 0);

    // Test handling of filenames with multiple dots
    assert(change_extension("archive.tar.gz", ".zip", output, sizeof(output)) == true);
    assert(strcmp(output, "archive.tar.zip") == 0);

    // Test output buffer being too small
    assert(change_extension("file.txt", ".verylongextension", output, 10) == false);

    // Test NULL arguments
    assert(change_extension(NULL, ".md", output, sizeof(output)) == false);
    assert(change_extension("file.txt", NULL, output, sizeof(output)) == false);
    assert(change_extension("file.txt", ".md", NULL, sizeof(output)) == false);
}