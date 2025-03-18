#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Assembles a given Hack assembly file into machine code.
 *
 * @param source_asm Pointer to the source assembly file (.asm), opened for reading.
 * @param source_filepath File path for source.asm file.
 * @param target_hack Pointer to the target machine code file (.hack), opened for writing.
 * @param print_tokens Bool that will be set true if token printing is enabled.
 * @return int 0 on success, non-zero on failure.
 */
int run_assembler(FILE *source_asm, char *source_filepath, FILE *target_hack, bool print_tokens);


#endif // ASSEMBLER_H

