#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256

typedef enum {
    A_INSTRUCTION,   // @value
    C_INSTRUCTION,   // dest=comp;jump
    L_INSTRUCTION,   // (LABEL)
    EMPTY,           // Empty or comment line
} InstructionType;

typedef struct {
    InstructionType type;
    char symbol[MAX_LINE_LENGTH];  // Symbol or value for A/L instructions
    char dest[4];  // Destination (e.g., "D")
    char comp[8];  // Computation (e.g., "D+1")
    char jump[4];  // Jump condition (e.g., "JGT")
} Instruction;

bool parse_line(const char *line, Instruction *instruction);

#endif