#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

/**
 * @brief Assembles a given Hack assembly file into machine code.
 *
 * @param source_asm Pointer to the source assembly file (.asm), opened for reading.
 * @param target_hack Pointer to the target machine code file (.hack), opened for writing.
 * @return int 0 on success, non-zero on failure.
 */
int run_assembler(FILE *source_asm, FILE *target_hack);


#endif // ASSEMBLER_H
