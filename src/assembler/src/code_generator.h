//
// Created by alexanderfisher on 08/03/25.
//
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "instruction.h"

/**
 * @brief Generates a binary representation of the given instruction.
 *
 * This function takes an instruction structure and converts it into a
 * 16-bit Hack assembly binary instruction. The resulting binary string
 * (null-terminated) is stored in the provided output buffer.
 *
 * @param instruction Pointer to the parsed instruction structure.
 * @param binary_output A character buffer to store the binary representation.
 */

void generate_binary(const Instruction *instruction, char *binary_output);

#endif // CODE_GENERATOR_H
