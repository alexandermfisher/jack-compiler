//
// Created by Alexander Fisher on 04/03/2025.
//
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <utils.h>
#include <sys/wait.h>


void test_parse_arguments(void);


int main(void) {
    test_parse_arguments();
    return 0;
}

void test_parse_arguments(void) {
    char *source_file;
    char *target_file;

    // Test Case 1: Basic usage (source file only)
    source_file = NULL;
    target_file = NULL;
    char *argv1[] = {"./hackasm", "source.asm"};
    parse_arguments(2, argv1, &source_file, &target_file);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(target_file == NULL);

    // Test Case 2: With output file specified
    source_file = NULL;
    target_file = NULL;
    char *argv2[] = {"./hackasm", "-o", "target.hack", "source.asm"};
    parse_arguments(4, argv2, &source_file, &target_file);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(strcmp(target_file, "target.hack") == 0);

    // Test Case 3: Alternative order (source first)
    source_file = NULL;
    target_file = NULL;
    char *argv3[] = {"./hackasm", "source.asm", "-o", "target.hack"};
    parse_arguments(4, argv3, &source_file, &target_file);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(strcmp(target_file, "target.hack") == 0);

    // Test Case 4: Missing source file (should exit)
    source_file = NULL;
    target_file = NULL;
    char *argv4[] = {"./hackasm"};
    if (fork() == 0) {
        parse_arguments(1, argv4, &source_file, &target_file);
        exit(EXIT_SUCCESS);
    }
    int status;
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 5: Missing output file after -o (should exit)
    source_file = NULL;
    target_file = NULL;
    char *argv5[] = {"./hackasm", "-o"};
    if (fork() == 0) {
        parse_arguments(2, argv5, &source_file, &target_file);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 6: Multiple -o options (should exit)
    source_file = NULL;
    target_file = NULL;
    char *argv6[] = {"./hackasm", "-o", "target1.hack", "-o", "target2.hack", "source.asm"};
    if (fork() == 0) {
        parse_arguments(6, argv6, &source_file, &target_file);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 7: Unrecognized extra argument (should exit)
    source_file = NULL;
    target_file = NULL;
    char *argv7[] = {"./hackasm", "source.asm", "unexpected_arg"};
    if (fork() == 0) {
        parse_arguments(3, argv7, &source_file, &target_file);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 8: Output file specified first (should exit)
    source_file = NULL;
    target_file = NULL;
    char *argv8[] = {"./hackasm", "-o", "target.hack"};
    if (fork() == 0) {
        parse_arguments(3, argv8, &source_file, &target_file);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    printf("\t✅ test_parse_arguments passed!\n");
}
