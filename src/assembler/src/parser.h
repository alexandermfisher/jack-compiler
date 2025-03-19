#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "symbol_table.h"
#include "token_table.h"
#include "instruction.h"

/**
 * Parser structure encapsulating the token table, symbol table, and current instruction.
 */
typedef struct {
    TokenTable *token_table;
    SymbolTable *symbol_table;
    Instruction *instruction;
} Parser;

/**
 * Creates and initializes a new Parser instance.
 *
 * @param token_table Pointer to the TokenTable containing the program tokens.
 * @param symbol_table Pointer to the SymbolTable for symbol resolution.
 * @return Pointer to the initialized Parser.
 */
Parser *parser_create(TokenTable *token_table, SymbolTable *symbol_table);

/**
 * Frees the Parser instance and any resources it owns.
 *
 * @param parser Pointer to the Parser to free.
 */
void parser_free(Parser *parser);

/**
 * Checks if there are more commands (instructions) to process in the token stream.
 *
 * Internally peeks ahead in the token table to determine if any tokens remain.
 *
 * @param parser Pointer to the Parser instance.
 * @return true if more commands exist, false otherwise.
 */
bool parser_has_more_commands(Parser *parser);

/**
 * Advances the parser to the next instruction.
 *
 * Reads tokens into an internal buffer, using newline tokens as instruction delimiters.
 * Parses the buffered tokens to fill the Instruction struct, preparing it for code generation.
 *
 * @param parser Pointer to the Parser instance.
 * @return true if the next instruction was successfully parsed, false if no more instructions exist.
 */
bool advance(Parser *parser);

#endif // PARSER_H
