//
// Created by Alexander Fisher on 04/03/2025.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <token.h>

#include "code_generator.h"  // Include your header for Instruction definition

int main(void) {
    char binary_output[17] = {0};

    // Test A-instruction with value 42
    Instruction a_instr = {.type = A_INSTRUCTION_VALUE, .value = 42};
    generate_binary(&a_instr, binary_output);
    assert(strcmp(binary_output, "0000000000101010") == 0);

    // Test A-instruction with value 0
    a_instr.value = 0;
    generate_binary(&a_instr, binary_output);
    assert(strcmp(binary_output, "0000000000000000") == 0);

    // Test C-instruction D = A
    Instruction c_instr = {
        .type = C_INSTRUCTION,
        .comp = TOKEN_COMP_A,
        .dest = TOKEN_DEST_D,
        .jump = TOKEN_JUMP_NULL
    };
    generate_binary(&c_instr, binary_output);
    assert(strcmp(binary_output, "1110110000010000") == 0);

    // Test C-instruction D;JGT
    c_instr.comp = TOKEN_COMP_D;
    c_instr.dest = TOKEN_DEST_NULL;
    c_instr.jump = TOKEN_JUMP_JGT;
    generate_binary(&c_instr, binary_output);
    assert(strcmp(binary_output, "1110001100000001") == 0);

    // Test invalid instruction
    Instruction invalid_instr = {.type = -1};
    generate_binary(&invalid_instr, binary_output);
    assert(strcmp(binary_output, "0000000000000000") == 0);

    printf("\t✅ test_code_generator passed!\n");
    return 0;
}
