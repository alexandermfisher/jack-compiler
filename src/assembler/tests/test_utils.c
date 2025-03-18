//
// Created by Alexander Fisher on 04/03/2025.
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
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
    bool print_tokens;

    int status;

    // Test Case 1: Basic usage (source file only)
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv1[] = {"./hackasm", "source.asm"};
    parse_arguments(2, argv1, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(target_file == NULL);
    assert(print_tokens == false);

    // Test Case 2: With output file specified
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv2[] = {"./hackasm", "-o", "target.hack", "source.asm"};
    parse_arguments(4, argv2, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(strcmp(target_file, "target.hack") == 0);
    assert(print_tokens == false);

    // Test Case 3: Token printing enabled
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv3[] = {"./hackasm", "-t", "source.asm"};
    parse_arguments(3, argv3, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(target_file == NULL);
    assert(print_tokens == true);

    // Test Case 4: Both -o and -t specified
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv4[] = {"./hackasm", "-o", "target.hack", "-t", "source.asm"};
    parse_arguments(5, argv4, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(strcmp(target_file, "target.hack") == 0);
    assert(print_tokens == true);

    // Test Case 5: Missing source file (should exit)
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv5[] = {"./hackasm", "-o", "target.hack"};
    if (fork() == 0) {
        parse_arguments(3, argv5, &source_file, &target_file, &print_tokens);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 6: Missing output file after -o (should exit)
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv6[] = {"./hackasm", "-o"};
    if (fork() == 0) {
        parse_arguments(2, argv6, &source_file, &target_file, &print_tokens);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 7: Multiple -o options (should exit)
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv7[] = {"./hackasm", "-o", "target1.hack", "-o", "target2.hack", "source.asm"};
    if (fork() == 0) {
        parse_arguments(6, argv7, &source_file, &target_file, &print_tokens);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 8: Unrecognized extra argument (should exit)
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv8[] = {"./hackasm", "-t", "source.asm", "unexpected"};
    if (fork() == 0) {
        parse_arguments(4, argv8, &source_file, &target_file, &print_tokens);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    // Test Case 9: -t in the middle
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv9[] = {"./hackasm", "-o", "target.hack", "-t", "source.asm"};
    parse_arguments(5, argv9, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "source.asm") == 0);
    assert(strcmp(target_file, "target.hack") == 0);
    assert(print_tokens == true);

    // Test Case 10: Source as first argument but still works
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv10[] = {"./hackasm", "source.asm", "-t"};
    if (fork() == 0) {
        parse_arguments(3, argv10, &source_file, &target_file, &print_tokens);
        assert(strcmp(source_file, "source.asm") == 0);
        assert(target_file == NULL);
        assert(print_tokens == true);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS);

    // NEW Test Case 11: Use of '--' to stop parsing options
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv11[] = {"./hackasm", "--", "-weirdsource.asm"};
    parse_arguments(3, argv11, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "-weirdsource.asm") == 0);
    assert(target_file == NULL);
    assert(print_tokens == false);

    // NEW Test Case 12: "--" with normal options before
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv12[] = {"./hackasm", "-t", "--", "-weirdsource.asm"};
    parse_arguments(4, argv12, &source_file, &target_file, &print_tokens);
    assert(strcmp(source_file, "-weirdsource.asm") == 0);
    assert(target_file == NULL);
    assert(print_tokens == true);

    // NEW Test Case 13: "--" but missing source file after it (should fail)
    source_file = NULL; target_file = NULL; print_tokens = false;
    char *argv13[] = {"./hackasm", "-t", "--"};
    if (fork() == 0) {
        parse_arguments(3, argv13, &source_file, &target_file, &print_tokens);
        exit(EXIT_SUCCESS);
    }
    wait(&status);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE);

    printf("\t✅ test_parse_arguments passed with -- support!\n");
}

