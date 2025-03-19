/**
 * Created by Alexander Fisher on 03/03/2025.
 *
 *
 * @brief Main entry point for the Hack Assembler (`hackasm`).
 *
 * @details
 * The assembler translates Hack Assembly (`.asm`) files into Hack Machine Code (`.hack`).
 * It processes command-line arguments to specify the source file, an optional target file,
 * and an optional flag to print tokens during processing.
 *
 * **Usage:**
 *   hackasm source.asm                   // Reads source.asm, writes to source.hack
 *   hackasm source.asm -o target.hack    // Writes to target.hack, reads source.asm
 *   hackasm source.asm -t                // Prints tokens during processing
 *   hackasm -o output.hack -t source.asm // Prints tokens and writes to output.hack
 *
 * **Command-line arguments:**
 *   - `source.asm` (required): The Hack assembly source file.
 *   - `-o target` or `--output target` (optional): Specify the target output filename.
 *     If omitted, `.hack` is added to the source filename.
 *   - `-t` or `--tokens` (optional): Enable printing of tokens during processing.
 *   - `--`: Stop argument parsing; all following arguments are positional.
 *
 * **Behavior:**
 *   - Generates the output file from the source file with proper extensions.
 *   - Reports errors for invalid filenames, missing source files, or incorrect usage.
 *   - Optionally prints tokens if the `-t` or `--tokens` flag is provided.
 *
 * **Examples:**
 *   hackasm add.asm                          → Generates `add.hack`
 *   hackasm -o my_output.hack add.asm        → Generates `my_output.hack`
 *   hackasm loop.asm -o custom.bin -t        → Generates `custom.bin` and prints tokens
 *   hackasm -t -o result.hack program.asm    → Generates `result.hack` and prints tokens
 */

#include <assembler.h>
#include <file_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "utils.h"

#define EXT_ASM ".asm"
#define EXT_HACK ".hack"


int main(const int argc, char *argv[]) {
    // Parse arguments
    char *source_file = NULL;
    char *target_file = NULL;
    bool print_tokens = false;
    parse_arguments(argc, argv, &source_file, &target_file, &print_tokens);

    // Validate source file extension
    if (!has_extension(source_file, EXT_ASM)) {
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
        if (!is_valid_filepath(target_file)) {
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

    // Open source file for reading
    FILE *source_file_ptr = fopen(source_file, "r");
    if (!source_file_ptr) {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    // Open target file for writing (creates a new file if it doesn't exist)
    FILE *target_file_ptr = fopen(target_file, "w");
    if (!target_file_ptr) {
        perror("Error opening target file");
        fclose(source_file_ptr);
        return EXIT_FAILURE;
    }

    // Run the assembler
    run_assembler(source_file_ptr, source_file, target_file_ptr, print_tokens);

    // Close files after processing
    fclose(source_file_ptr);
    fclose(target_file_ptr);

    return EXIT_SUCCESS;
}
