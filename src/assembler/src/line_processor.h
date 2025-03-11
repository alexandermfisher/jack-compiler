//
// Created by Alexander Fisher on 09/03/2025.
//

#ifndef LINE_PROCESSOR_H
#define LINE_PROCESSOR_H

#include "token_table.h"
#include "symbol_table.h"


typedef enum {
    PROCESS_SUCCESS,   // Successfully processed a valid line
    PROCESS_INVALID,   // Syntax error in the line
    PROCESS_ERROR      // Critical failure (e.g., malloc failure)
} ProcessStatus;

const char *preprocess_line(const char *line);

bool is_keyword(const char *symbol);

bool is_line_end_or_comment(const char *line);

ProcessStatus process_line(const char *line, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address);

ProcessStatus process_label(const char **line, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address);

ProcessStatus process_symbol(const char **line, char *buffer);

ProcessStatus process_integer_literal(const char **line, int *integer_literal);

ProcessStatus process_a_instruction(const char **line, TokenTable *token_table);




#endif //LINE_PROCESSOR_H
