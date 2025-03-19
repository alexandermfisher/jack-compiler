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

// Instruction structure
typedef struct {
    InstructionType type;
    union {
        char *symbol;       // For L-instructions and symbolic A-instructions
        int value;          // For numeric A-instructions (@40, @100)
    };
    int dest;               // TOKEN_DEST_*
    int comp;               // TOKEN_COMP_*
    int jump;               // TOKEN_JUMP_*
} Instruction;

#endif // INSTRUCTION_H
