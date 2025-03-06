//
// Created by alexanderfisher on 06/03/25.
//

#include "utils.h"

void parse_arguments(const int argc, char *argv[], char **source_file, char **target_file) {
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-o") == 0) {
            // Optional Argument: -o <target_file>
            if (i + 1 < argc) {
                if (*target_file != NULL) {
                    fprintf(stderr, "Error: Multiple -o options are not allowed.\n");
                    fprintf(stderr, "Usage: %s [-o output.hack] source.asm\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                *target_file = argv[++i];
            } else {
                fprintf(stderr, "Error: -o requires a target file.\n");
                fprintf(stderr, "Usage: %s [-o output.hack] source.asm\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (*source_file == NULL) {
            // Positional argument: <source_file>
            *source_file = argv[i];
        } else {
            fprintf(stderr, "Error: Unrecognised argument '%s'.\n", argv[i]);
            fprintf(stderr, "Usage: %s [-o output.hack] source.asm\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        i++;
    }

    // **Final Check: Ensure a source file was provided**
    if (*source_file == NULL) {
        fprintf(stderr, "Error: No source file provided.\n");
        fprintf(stderr, "Usage: %s [-o output.hack] source.asm\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}