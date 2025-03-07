#include "assembler.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int run_assembler(const char *input_asm, const char *output_hack) {
    // validate file extensions:
    if (!input_asm || !output_hack) {
        fprintf(stderr, "Error: NULL input or output file path.\n");
        return EXIT_FAILURE;
    }
    // open files:

    // Initialise Parser

    // Loop through each line and generate code

    return 0;
}
