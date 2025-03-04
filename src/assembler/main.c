/**
 * Main entry point for the Hack Assembler (`hackasm`).
 *
 * Usage:
 *   hackasm input.asm                // Reads input.asm, writes to input.hack
 *   hackasm input.asm -o output.hack // Reads input.asm, writes to output.hack
 *
 * Command-line arguments:
 *   - `input.asm` (required): Assembly source file.
 *   - `-o output.hack` (optional): Specify output file.
 *
 * Behavior:
 *   - Input file **must** be provided (no stdin support).
 *   - If no `-o` option is given, output filename is **the same as input, but with `.hack` extension**.
 *   - Prints an error message for incorrect usage.
 *
 * Examples:
 *   hackasm add.asm       → Generates `add.hack`
 *   hackasm multiply.asm  → Generates `multiply.hack`
 *   hackasm loop.asm -o my_output.hack → Generates `my_output.hack`
 */

#include "assembler.h"
#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXT_ASM ".asm"
#define EXT_ASM_LEN 4
#define EXT_HACK ".hack"


int main(const int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file.asm>\n", argv[0]);
        return EXIT_FAILURE;
    }





    const char *source_file = argv[1];
    const size_t source_file_len = strlen(source_file);

    // validate source file extension:
    if (!has_extension(source_file,EXT_ASM)) {
        fprintf(stderr, "Error: Input file must have a .asm extension\n");
        return EXIT_FAILURE;
    }



    // Create output filename (replace .asm with .hack)
    char hack_file[256];
    strncpy(hack_file, source_file, source_file_len - EXT_ASM_LEN);
    hack_file[source_file_len - EXT_ASM_LEN] = '\0';
    strcat(hack_file, EXT_HACK);

    return run_assembler(source_file, hack_file);
}

// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         printf("Usage: %s <input.asm> [output.hack]\n", argv[0]);
//         return 1;
//     }
//
//     char output_filename[256];
//
//     if (argc == 3) {
//         // Use user-provided filename
//         snprintf(output_filename, sizeof(output_filename), "%s", argv[2]);
//     } else {
//         // Automatically generate output filename
//         if (!change_extension(argv[1], ".hack", output_filename, sizeof(output_filename))) {
//             printf("Error: Filename too long\n");
//             return 1;
//         }
//     }
//
//     printf("Assembling %s to %s\n", argv[1], output_filename);
//
//     return 0;
// }