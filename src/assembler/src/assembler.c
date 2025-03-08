#include "assembler.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int run_assembler(FILE *source_asm, FILE *target_hack) {
    // validate file extensions:
    if (!source_asm || !target_hack) {
        fprintf(stderr, "Error: NULL input or output file path.\n");
        return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, source_asm)) != -1) {
        printf("%s", line);  // Print each line as read
    }

    free(line);  // Free memory allocated by getline

    // Initialise Parser

    // Loop through each line and generate code

    return 0;
}
