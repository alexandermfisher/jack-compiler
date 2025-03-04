// test_main.c
#include <stdio.h>

int test_parser();
int test_code();
int test_symbol_table();

int main() {
    int failed = 0;

    failed += test_parser();
    failed += test_code();
    failed += test_symbol_table();

    if (failed == 0) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("%d test(s) failed.\n", failed);
        return 1;
    }
}