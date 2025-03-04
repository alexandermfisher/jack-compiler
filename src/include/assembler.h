#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

/**
 * @brief Assembles a given Hack assembly file into machine code.
 *
 * @param input_asm Path to the input assembly file (.asm).
 * @param output_hack Path to the output machine code file (.hack).
 * @return int 0 on success, non-zero on failure.
 */
int run_assembler(const char *input_asm, const char *output_hack);

#endif // ASSEMBLER_H