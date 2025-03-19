#include "code_generator.h"
#include <stdio.h>
#include <string.h>
#include "token_table.h"

const char* encode_comp(int comp);
const char* encode_dest(int dest);
const char* encode_jump(int jump);

// Function to generate binary code for an instruction
void generate_binary(const Instruction *instruction, char *binary_output) {
    if (instruction->type == A_INSTRUCTION_VALUE) {
        // A-instruction: 0 + 15-bit address
        for (int i = 15; i >= 0; i--) {
            binary_output[15 - i] = (instruction->value & (1 << i)) ? '1' : '0';
        }
    } else if (instruction->type == C_INSTRUCTION) {
        // C-instruction: 111 + comp + dest + jump
        snprintf(binary_output, 17, "111%s%s%s",
                 encode_comp(instruction->comp),
                 encode_dest(instruction->dest),
                 encode_jump(instruction->jump));
    } else {
        // Invalid instruction
        strcpy(binary_output, "0000000000000000");
    }
}

const char* encode_comp(const int comp) {
    switch (comp) {
        case TOKEN_COMP_0: return "0101010";
        case TOKEN_COMP_1: return "0111111";
        case TOKEN_COMP_NEG1: return "0111010";
        case TOKEN_COMP_D: return "0001100";
        case TOKEN_COMP_A: return "0110000";
        case TOKEN_COMP_NOT_D: return "0001101";
        case TOKEN_COMP_NOT_A: return "0110001";
        case TOKEN_COMP_NEG_D: return "0001111";
        case TOKEN_COMP_NEG_A: return "0110011";
        case TOKEN_COMP_DPLUS1: return "0011111";
        case TOKEN_COMP_APLUS1: return "0110111";
        case TOKEN_COMP_DMINUS1: return "0001110";
        case TOKEN_COMP_AMINUS1: return "0110010";
        case TOKEN_COMP_DPLUSA: return "0000010";
        case TOKEN_COMP_DMINUSA: return "0010011";
        case TOKEN_COMP_AMINUSD: return "0000111";
        case TOKEN_COMP_DANDA: return "0000000";
        case TOKEN_COMP_DORA: return "0010101";
        case TOKEN_COMP_M: return "1110000";
        case TOKEN_COMP_NOT_M: return "1110001";
        case TOKEN_COMP_NEG_M: return "1110011";
        case TOKEN_COMP_MPLUS1: return "1110111";
        case TOKEN_COMP_MMINUS1: return "1110010";
        case TOKEN_COMP_DPLUSM: return "1000010";
        case TOKEN_COMP_DMINUSM: return "1010011";
        case TOKEN_COMP_MMINUSD: return "1000111";
        case TOKEN_COMP_DANDM: return "1000000";
        case TOKEN_COMP_DORM: return "1010101";
        default: return "0000000";  // Invalid case
    }
}

const char* encode_dest(const int dest) {
    switch (dest) {
        case TOKEN_DEST_NULL: return "000";
        case TOKEN_DEST_M: return "001";
        case TOKEN_DEST_D: return "010";
        case TOKEN_DEST_MD: return "011";
        case TOKEN_DEST_A: return "100";
        case TOKEN_DEST_AM: return "101";
        case TOKEN_DEST_AD: return "110";
        case TOKEN_DEST_AMD: return "111";
        default: return "000";  // Invalid case
    }
}

const char* encode_jump(const int jump) {
    switch (jump) {
        case TOKEN_JUMP_NULL: return "000";
        case TOKEN_JUMP_JGT: return "001";
        case TOKEN_JUMP_JEQ: return "010";
        case TOKEN_JUMP_JGE: return "011";
        case TOKEN_JUMP_JLT: return "100";
        case TOKEN_JUMP_JNE: return "101";
        case TOKEN_JUMP_JLE: return "110";
        case TOKEN_JUMP_JMP: return "111";
        default: return "000";  // Invalid case
    }
}
