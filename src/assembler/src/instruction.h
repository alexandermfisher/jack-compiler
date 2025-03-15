#ifndef INSTRUCTION_H
#define INSTRUCTION_H


// Enum for instruction types
typedef enum {
    A_INSTRUCTION_SYMBOL,  // @LABEL
    A_INSTRUCTION_VALUE,   // @40, @100
    C_INSTRUCTION,         // dest=comp;jump
    L_INSTRUCTION,         // (LABEL)
    INVALID_INSTRUCTION
} InstructionType;


// Enum for DEST field
typedef enum {
    NULL_DEST = 0,                  // No destination (e.g., just comp;jump)
    DEST_M    = 1 << 0,             // 0001 -> M
    DEST_D    = 1 << 1,             // 0010 -> D
    DEST_A    = 1 << 2,             // 0100 -> A
    DEST_MD   = DEST_M | DEST_D,    // 0011 -> MD
    DEST_AM   = DEST_A | DEST_M,    // 0101 -> AM
    DEST_AD   = DEST_A | DEST_D,    // 0110 -> AD
    DEST_AMD  = DEST_A | DEST_M | DEST_D  // 0111 -> AMD
} Dest;


// Enum for JUMP field
typedef enum {NO_JUMP, JGT, JEQ, JGE, JLT, JNE, JLE, JMP} Jump;


// Enum for COMP field
typedef enum {
    COMP_0, COMP_1, COMP_MINUS1, COMP_D, COMP_A, COMP_NOT_D, COMP_NOT_A,
    COMP_MINUS_D, COMP_MINUS_A, COMP_D_PLUS_1, COMP_A_PLUS_1, COMP_D_MINUS_1,
    COMP_A_MINUS_1, COMP_D_PLUS_A, COMP_D_MINUS_A, COMP_A_MINUS_D, COMP_D_AND_A,
    COMP_D_OR_A, COMP_M, COMP_NOT_M, COMP_MINUS_M, COMP_M_PLUS_1, COMP_M_MINUS_1,
    COMP_D_PLUS_M, COMP_D_MINUS_M, COMP_M_MINUS_D, COMP_D_AND_M, COMP_D_OR_M
} Comp;


// Instruction structure
typedef struct {
    InstructionType type;
    union {
        char *symbol;       // For L-instructions and symbolic A-instructions
        int value;          // For numeric A-instructions (@40, @100)
    };
    Dest dest;
    Comp comp;
    Jump jump;
} Instruction;


// Function to encode COMP field into binary
inline const char* encode_comp(const Comp comp) {
    switch (comp) {
        case COMP_0: return "0101010";
        case COMP_1: return "0111111";
        case COMP_MINUS1: return "0111010";
        case COMP_D: return "0001100";
        case COMP_A: return "0110000";
        case COMP_NOT_D: return "0001101";
        case COMP_NOT_A: return "0110001";
        case COMP_MINUS_D: return "0001111";
        case COMP_MINUS_A: return "0110011";
        case COMP_D_PLUS_1: return "0011111";
        case COMP_A_PLUS_1: return "0110111";
        case COMP_D_MINUS_1: return "0001110";
        case COMP_A_MINUS_1: return "0110010";
        case COMP_D_PLUS_A: return "0000010";
        case COMP_D_MINUS_A: return "0010011";
        case COMP_A_MINUS_D: return "0000111";
        case COMP_D_AND_A: return "0000000";
        case COMP_D_OR_A: return "0010101";
        case COMP_M: return "1110000";
        case COMP_NOT_M: return "1110001";
        case COMP_MINUS_M: return "1110011";
        case COMP_M_PLUS_1: return "1110111";
        case COMP_M_MINUS_1: return "1110010";
        case COMP_D_PLUS_M: return "1000010";
        case COMP_D_MINUS_M: return "1010011";
        case COMP_M_MINUS_D: return "1000111";
        case COMP_D_AND_M: return "1000000";
        case COMP_D_OR_M: return "1010101";
        default: return "0000000";  // Invalid case
    }
}

// // Function to encode DEST field into binary
inline const char* encode_dest(const Dest dest) {
    switch (dest) {
        case NULL_DEST: return "000";
        case DEST_M: return "001";
        case DEST_D: return "010";
        case DEST_MD: return "011";
        case DEST_A: return "100";
        case DEST_AM: return "101";
        case DEST_AD: return "110";
        case DEST_AMD: return "111";
        default: return "000";  // Invalid case
    }
}

// Function to encode JUMP field into binary
inline const char* encode_jump(const Jump jump) {
    switch (jump) {
        case NO_JUMP: return "000";
        case JGT: return "001";
        case JEQ: return "010";
        case JGE: return "011";
        case JLT: return "100";
        case JNE: return "101";
        case JLE: return "110";
        case JMP: return "111";
        default: return "000";  // Invalid case
    }
}

#endif // INSTRUCTION_H
