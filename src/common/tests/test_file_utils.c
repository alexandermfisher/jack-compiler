//
// Created by Alexander Fisher on 03/03/2025.
//
#include "file_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void test_has_extension(void);
void test_change_file_extension(void);
void test_is_valid_filepath(void);


int main(void) {
    test_has_extension();
    test_change_file_extension();
    test_is_valid_filepath();
    return 0;
}

void test_has_extension(void) {
    // Test basic cases
    assert(has_extension("source.asm", ".asm") == true);
    assert(has_extension("source.hack", ".asm") == false);
    assert(has_extension("source", ".asm") == false);

    // Test simple filenames
    assert(has_extension("source.text.asm", ".asm") == true);
    assert(has_extension("source.asm.asm", ".asm") == true);
    assert(has_extension("source.asmnot", ".asm") == false);
    assert(has_extension("source.asm.text", ".asm") == false);

    // Test full paths
    assert(has_extension("file/path/source.asm", ".asm") == true);
    assert(has_extension("file/path/source.text.asm", ".asm") == true);
    assert(has_extension("file/path/source.asm.asm", ".asm") == true);
    assert(has_extension("file/path/source.asmnot", ".asm") == false);
    assert(has_extension("file/path/source.asm.text", ".asm") == false);
    assert(has_extension("file/path/source.text.asm", ".text") == false);

    // Test relative paths
    assert(has_extension("file/../source.asm", ".asm") == true);
    assert(has_extension("file/../source.text.asm", ".asm") == true);
    assert(has_extension("file/../source.text.asm", ".text") == false);
    assert(has_extension("file/../source.asm.asm", ".asm.asm") == false);

    // Test NULL values
    assert(has_extension(NULL, ".asm") == false);
    assert(has_extension("test.asm", NULL) == false);

    printf("        ✅ test_has_extension passed!\n");
}

void test_change_file_extension(void) {
    char filepath[256];

    // --- Basic Cases ---
    // Replacing an extension
    strcpy(filepath, "file.txt");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, "file.md") == 0);

    // Appending an extension when none exists
    strcpy(filepath, "file");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, "file.md") == 0);

    // Handling multiple dots in filenames
    strcpy(filepath, "archive.tar.gz");
    assert(change_file_extension(filepath, sizeof(filepath), ".zip") == true);
    assert(strcmp(filepath, "archive.tar.zip") == 0);

    // --- Buffer Size Checks ---
    // Output buffer too small
    strcpy(filepath, "file.txt");
    assert(change_file_extension(filepath, 10, ".verylongextension") == false);

    // --- Invalid Inputs ---
    // NULL arguments
    assert(change_file_extension(NULL, sizeof(filepath), ".md") == false);
    assert(change_file_extension(filepath, sizeof(filepath), NULL) == false);

    // Empty string as filename (invalid)
    strcpy(filepath, "");
    assert(change_file_extension(filepath, sizeof(filepath), ".txt") == false);

    // Special filenames (should be rejected)
    strcpy(filepath, ".");
    assert(change_file_extension(filepath, sizeof(filepath), ".txt") == false);

    strcpy(filepath, "..");
    assert(change_file_extension(filepath, sizeof(filepath), ".txt") == false);

    // --- Path Handling ---
    // Absolute paths
    strcpy(filepath, "/home/user/file.txt");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, "/home/user/file.md") == 0);

    // Relative paths
    strcpy(filepath, "./folder/file.txt");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, "./folder/file.md") == 0);

    // Going up directories
    strcpy(filepath, "../file.txt");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, "../file.md") == 0);

    // Multiple path segments
    strcpy(filepath, "dir/subdir/file.txt");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, "dir/subdir/file.md") == 0);

    // --- Hidden Files ---
    // Hidden files with no extension
    strcpy(filepath, ".hiddenfile");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == true);
    assert(strcmp(filepath, ".hiddenfile.md") == 0);

    // Hidden files with existing extensions
    strcpy(filepath, ".configrc.txt");
    assert(change_file_extension(filepath, sizeof(filepath), ".bak") == true);
    assert(strcmp(filepath, ".configrc.bak") == 0);

    // Hidden files with no base name (should be rejected)
    strcpy(filepath, ".");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == false);
    strcpy(filepath, "..");
    assert(change_file_extension(filepath, sizeof(filepath), ".md") == false);

    // --- Edge Cases ---
    // File already has the desired extension (should keep as-is)
    strcpy(filepath, "script.sh");
    assert(change_file_extension(filepath, sizeof(filepath), ".sh") == true);
    assert(strcmp(filepath, "script.sh") == 0);

    // Filename is just an extension (e.g., ".gitignore" → should append)
    strcpy(filepath, ".gitignore");
    assert(change_file_extension(filepath, sizeof(filepath), ".bak") == true);
    assert(strcmp(filepath, ".gitignore.bak") == 0);

    // Filenames with trailing dots (invalid edge case, should trim dots)
    strcpy(filepath, "invalidfile.");
    assert(change_file_extension(filepath, sizeof(filepath), ".txt") == true);
    assert(strcmp(filepath, "invalidfile.txt") == 0);

    // Filename with multiple trailing dots
    strcpy(filepath, "weirdfile....");
    assert(change_file_extension(filepath, sizeof(filepath), ".txt") == true);
    assert(strcmp(filepath, "weirdfile....txt") == 0);

    // File with only an extension (e.g., ".env" should become ".env.bak")
    strcpy(filepath, ".env");
    assert(change_file_extension(filepath, sizeof(filepath), ".bak") == true);
    assert(strcmp(filepath, ".env.bak") == 0);

    printf("        ✅ test_change_file_extension passed!\n");
}

void test_is_valid_filepath(void) {
    // Valid file paths
    assert(is_valid_filepath("valid_file.txt") == true);
    assert(is_valid_filepath("another_valid-file") == true);
    assert(is_valid_filepath("file_with_numbers_123") == true);
    assert(is_valid_filepath("UPPERCASE") == true);
    assert(is_valid_filepath("mixED.case") == true);
    assert(is_valid_filepath("spaces are okay") == true);
    assert(is_valid_filepath("subdir/file.txt") == true);
    assert(is_valid_filepath("file/name") == true);
    assert(is_valid_filepath("./relative/path/to/file") == true);
    assert(is_valid_filepath("../parent_dir/file") == true);
    assert(is_valid_filepath("/absolute/path/to/file.txt") == true);

    // Invalid cases
    assert(is_valid_filepath("") == false);            // Empty string
    assert(is_valid_filepath(".") == false);           // Single dot
    assert(is_valid_filepath("..") == false);          // Double dots
    assert(is_valid_filepath("/") == false);           // Root directory
    assert(is_valid_filepath("/home/user/") == false); // Ends with '/', indicating a directory
    assert(is_valid_filepath("trailing_slash/") == false);

    // Overly long filename (should be false if it exceeds PATH_MAX)
    char long_filename[PATH_MAX + 2];
    memset(long_filename, 'a', PATH_MAX + 1);
    long_filename[PATH_MAX + 1] = '\0';
    assert(is_valid_filepath(long_filename) == false);

    printf("        ✅ test_is_valid_filepath passed!\n");
}
