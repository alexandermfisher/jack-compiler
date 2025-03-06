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
 *   hackasm source.asm -o target.hack // Writes to target.hack, reads source.asm
 *
 * **Command-line arguments:**
 *   - `source.asm` (required): The Hack assembly source file.
 *   - `-o target` (optional): Specify the target filename.
 *       - If `target` has no extension, `.hack` is automatically appended.
 *       - If `target` includes an extension, it is preserved as-is.
 *
 * **Behavior:**
 *   - If `-o` is used, it **must** be the first argument.
 *   - If `-o` is omitted, the output file defaults to `source.hack`.
 *   - The source file **must** always be provided as the last argument.
 *   - An error message is printed for incorrect usage.
 *
 * **Examples:**
 *   hackasm add.asm                   → Generates `add.hack`
 *   hackasm -o my_output.hack add.asm → Generates `my_output.hack`
 *   hackasm -o custom.bin loop.asm   → Generates `custom.bin`
 *   hackasm loop.asm -o target.hack  → Error (incorrect argument order)
 */

#include <file_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "utils.h"

#define EXT_ASM ".asm"
#define EXT_HACK ".hack"


int main(const int argc, char *argv[]) {
    // Parse arguments
    char *source_file = NULL;
    char *target_file = NULL;
    parse_arguments(argc, argv, &source_file, &target_file);

    if (!is_valid_filename(source_file)) {
        fprintf(stderr, "Error: Invalid source filename.\n");
        return EXIT_FAILURE;
    }

    // Validate source file extension
    if (has_extension(source_file, EXT_ASM)) {
        fprintf(stderr, "Error: Source file must have '.asm' extension.\n");
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
        // Generate default target filename (name.asm -> name.hack)
        static char default_target[PATH_MAX];
        const char *slash = strrchr(source_file, '/');
        const char *filename = (slash) ? slash + 1 : source_file;
        strncpy(default_target, filename, PATH_MAX);
        if (!change_file_extension(default_target, PATH_MAX, EXT_HACK)) {
            fprintf(stderr, "Error: Unable to generate target filename from.\n");
            return EXIT_FAILURE;
        }
        target_file = default_target;
    }

    // Prevent overwriting source file
    if (strcmp(source_file, target_file) == 0) {
        fprintf(stderr, "Error: Output file cannot be the same as source file.\n");
        return EXIT_FAILURE;
    }

    printf("Assembling: %s → %s\n", source_file, target_file);

    return EXIT_SUCCESS;
}




