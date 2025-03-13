#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>

#include "config.h"
#include "symbol_table.h"
#include "token_table.h"


typedef enum {
    A_INSTRUCTION_SYMBOL,  // @LABEL
    A_INSTRUCTION_VALUE,   // @40, @100
    C_INSTRUCTION,         // dest=comp;jump
    L_INSTRUCTION,         // (LABEL)
    INVALID_INSTRUCTION
} InstructionType;


typedef struct {
    InstructionType type;
    union {
        char *symbol;  // For L-instructions and symbolic A-instructions
        int value;                     // For numeric A-instructions (@40, @100, etc.)
    };
    char dest[4];  // Destination (e.g., "D")
    char comp[8];  // Computation (e.g., "D+1")
    char jump[4];  // Jump condition (e.g., "JGT")
} Instruction;


typedef struct {
    TokenTable *token_table;
    SymbolTable *symbol_table;
    Instruction *instruction;
} Parser;


Parser *parser_create(TokenTable *token_table, SymbolTable *symbol_table);

void parser_free(Parser *parser);

bool parser_has_more_commands(Parser *parser);

bool advance(Parser *parser);

#endif
