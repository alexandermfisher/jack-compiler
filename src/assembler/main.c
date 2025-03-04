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
#include <stdlib.h>
#include <string.h>
#include <limits.h> // For PATH_MAX
#include "file_utils.h"

#define EXT_ASM ".asm"
#define EXT_HACK ".hack"


int main(int argc, char *argv[]) {
    char source_filename[PATH_MAX];
    char target_filename[PATH_MAX];

    // Ensure correct number of arguments
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Usage: %s [-o <target>] <source.asm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Case 1: No `-o`, use default output filename
    if (argc == 2) {
        // Copy source filename and validate its extension
        strncpy(source_filename, argv[1], PATH_MAX - 1);
        source_filename[PATH_MAX - 1] = '\0';

        if (!has_extension(source_filename, EXT_ASM)) {
            fprintf(stderr, "Error: Source file must have a .asm extension.\n");
            return EXIT_FAILURE;
        }

        // Default target filename (same as source but with .hack extension)
        if (!change_extension(source_filename, ".hack", target_filename, PATH_MAX)) {
            fprintf(stderr, "Error: Failed to set output filename.\n");
            return EXIT_FAILURE;
        } // Case 2: `-o <target>` specified
    } else {
        // Validate argument order (-o must be first)



    }
}







// int main(int argc, char *argv[]) {
//     // Case 2: `-o <target>` specified
//     else {
//         // Validate argument order (-o must be first)
//         if (strcmp(argv[1], "-o") != 0) {
//             fprintf(stderr, "Error: Invalid argument order. Expected: %s -o <target> <source.asm>\n", argv[0]);
//             return EXIT_FAILURE;
//         }
//
//         // Copy target filename from second argument
//         strncpy(target_filename, argv[2], PATH_MAX - 1);
//         target_filename[PATH_MAX - 1] = '\0';
//
//         // Copy source filename from third argument
//         strncpy(source_filename, argv[3], PATH_MAX - 1);
//         source_filename[PATH_MAX - 1] = '\0';
//
//         // Validate source file extension
//         if (!has_extension(source_filename, ".asm")) {
//             fprintf(stderr, "Error: Source file must have a .asm extension.\n");
//             return EXIT_FAILURE;
//         }
//
//         // If target filename has no extension, append .hack
//         if (!strchr(target_filename, '.')) {
//             if (!change_extension(target_filename, ".hack", target_filename, PATH_MAX)) {
//                 fprintf(stderr, "Error: Failed to set output filename.\n");
//                 return EXIT_FAILURE;
//             }
//         }
//     }
//
//     printf("Assembling: %s → %s\n", source_filename, target_filename);
//
//     // TODO: Open files and run assembler logic here
//
//     return EXIT_SUCCESS;
// }