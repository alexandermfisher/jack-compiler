#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "symbol_table.h"
#include "token_table.h"
#include "instruction.h"


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
