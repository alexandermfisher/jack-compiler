//
// Created by alexanderfisher on 16/03/25.
//

#ifndef LEXER_H
#define LEXER_H

#include "token_table.h"
#include "symbol_table.h"

typedef enum {
    PROCESS_SUCCESS,   // Successfully processed a valid line
    PROCESS_INVALID,   // Syntax error in the line
    PROCESS_ERROR      // Critical failure (e.g., malloc failure)
} ProcessStatus;


/**
 * @brief Tokenizes and processes a single line of assembly code.
 *
 * This function performs lexical analysis on an assembly language line, extracting tokens
 * and adding them to the TokenTable. It also processes symbols, adding labels to the SymbolTable where necessary.
 * If the line represents an instruction, the ROM address is updated. Additionally, it validates syntax and
 * assigns meaning via the extracted tokens.
 *
 * @param line         The assembly source line (read-only).
 * @param read         Length of the line (used for memory allocation).
 * @param token_table  A pointer to the TokenTable where tokens will be stored.
 * @param symbol_table A pointer to the SymbolTable for tracking symbols and labels.
 * @param rom_address  A pointer to the ROM address counter, updated for instruction lines.
 *
 * @return A ProcessStatus enum indicating the result of processing:
 *         - PROCESS_SUCCESS: Line processed successfully.
 *         - PROCESS_INVALID: Syntax error detected.
 *         - PROCESS_ERROR:   Critical failure (e.g., memory allocation failure).
 */
ProcessStatus lex_line(char *line, ssize_t read, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address);


#endif //LEXER_H
