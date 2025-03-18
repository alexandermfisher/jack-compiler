//
// Created by alexanderfisher on 06/03/25.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_arguments(const int argc, char *argv[], char **source_file, char **target_file, bool *print_tokens) {
    int i = 1;
    bool end_of_options = false;

    while (i < argc) {
        if (!end_of_options && strcmp(argv[i], "--") == 0) {
            // End of options, treat all following arguments as positional
            end_of_options = true;
            i++;
            continue;
        }

        if (!end_of_options && (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)) {
            // Optional Argument: -o <target_file>
            if (i + 1 < argc) {
                if (*target_file != NULL) {
                    fprintf(stderr, "Error: Multiple -o options are not allowed.\n");
                    fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                *target_file = argv[++i];
            } else {
                fprintf(stderr, "Error: -o requires a target file.\n");
                fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (!end_of_options && (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tokens") == 0)) {
            // Toggle printing of tokens
            *print_tokens = true;
        } else if (*source_file == NULL) {
            // Positional argument: <source_file>
            *source_file = argv[i];
        } else {
            fprintf(stderr, "Error: Unrecognized argument '%s'.\n", argv[i]);
            fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        i++;
    }

    if (*source_file == NULL) {
        fprintf(stderr, "Error: Source file is required.\n");
        fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}


