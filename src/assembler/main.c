/**
 * Created by Alexander Fisher on 03/03/2025.
 *
 *
 * @brief Main entry point for the Hack Assembler (`hackasm`).
 *
 * @details
 * This assembler translates Hack Assembly (`.asm`) files into Hack Machine Code (`.hack`).
 * The command-line arguments follow a **GCC-style order**.
 *
 * **Usage:**
 *   hackasm source.asm                // Reads source.asm, writes to source.hack
 *   hackasm -o target.hack source.asm // Writes to target.hack, reads source.asm
 *
 * **Command-line arguments (must be in this order):**
 *   - `-o target` (optional): Specify the target filename.
 *       - If `target` has no extension, `.hack` is automatically appended.
 *       - If `target` includes an extension, it is preserved as-is.
 *   - `source.asm` (required): The Hack assembly source file.
 *
 * **Behavior:**
 *   - If `-o` is used, it **must** be the first argument.
 *   - If `-o` is omitted, the output file defaults to `source.hack`.
 *   - The source file **must** always be provided as the last argument.
 *   - An error message is printed for incorrect usage.
 *
 * **Examples:**
 *   hackasm add.asm                  → Generates `add.hack`
 *   hackasm -o my_output.hack add.asm → Generates `my_output.hack`
 *   hackasm -o custom.bin loop.asm   → Generates `custom.bin`
 *   hackasm loop.asm -o target.hack  → Error (incorrect argument order)
 */

#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define EXT_ASM ".asm"
#define EXT_HACK ".hack"

int is_valid_filename(const char *filename) {
    return filename && strlen(filename) > 0 && strlen(filename) < PATH_MAX;
}

bool parse_arguments(int argc, char *argv[], char **source_file, char **target_file);

int main(const int argc, char *argv[]) {
    char *source_file = NULL;
    char *target_file = NULL;

    // parse_arguments(argc, argv, &source_file, &target_file);
    // printf("Source: %s\nTarget: %s\n", source_file, target_file);

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                target_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing output filename after '-o'.\n");
                return EXIT_FAILURE;
            }
        } else if (!source_file) {
            source_file = argv[i];
        } else {
            fprintf(stderr, "Error: Unexpected argument '%s'.\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    printf("source: %s\ntarget: %s\n", source_file,  target_file);
    // Check if source file is provided
    if (!source_file) {
        fprintf(stderr, "Error: No source file provided.\n");
        fprintf(stderr, "Usage: %s [-o output.hack] source.asm\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Validate source file extension
    if (strstr(source_file, ".asm") == NULL) {
        fprintf(stderr, "Error: Source file must have a .asm extension.\n");
        return EXIT_FAILURE;
    }

    // Check if source file exists
    if (access(source_file, F_OK) != 0) {
        fprintf(stderr, "Error: Source file '%s' does not exist.\n", source_file);
        return EXIT_FAILURE;
    }

    // Validate output filename (if provided)
    if (target_file) {
        if (!is_valid_filename(target_file)) {
            fprintf(stderr, "Error: Invalid output filename.\n");
            return EXIT_FAILURE;
        }
    } else {
        // Generate default output filename (source.asm -> source.hack)
        static char default_output[PATH_MAX];
        strncpy(default_output, source_file, PATH_MAX);
        char *dot = strrchr(default_output, '.');
        if (dot) *dot = '\0';  // Remove existing extension
        strcat(default_output, ".hack");
        target_file = default_output;
    }

    // Prevent overwriting source file
    if (strcmp(source_file, target_file) == 0) {
        fprintf(stderr, "Error: Output file cannot be the same as source file.\n");
        return EXIT_FAILURE;
    }

    printf("Assembling: %s → %s\n", source_file, target_file);

    // TODO: Run assembler logic here...

    return EXIT_SUCCESS;
}


bool parse_arguments(const int argc, char *argv[], char **source_file, char **target_file) {

    if (argc != 2 && argc != 4) {
        return false;
    }

    if (argc == 2) {
        *source_file = argv[1];
        return true;
    }

    if (strcmp(argv[1], "-o") != 0) {
        return false;
    }

    *target_file = argv[2];
    *source_file = argv[3];

    return true;
}
