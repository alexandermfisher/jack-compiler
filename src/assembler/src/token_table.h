//
// Created by Alexander Fisher on 09/03/2025.
//

#ifndef TOKEN_TABLE_H
#define TOKEN_TABLE_H

#include <stdbool.h>
#include "token.h"
#include <stdio.h>

// Declare TokenTable as an opaque type
typedef struct TokenTable TokenTable;

// Create a new token table
TokenTable *token_table_create(void);

// Add a token to the table
bool token_table_add(TokenTable *table, Token *token);

// Get the next token (iterator-style)
Token *token_table_next(TokenTable *table);

// Get the next token without incrementing current ptr
Token *token_table_peek(TokenTable *table);

// Reset the iterator to the start
void token_table_reset(TokenTable *table);

// Free the entire token table
void token_table_free(TokenTable *table);

// Write tokens to file:
void token_table_write_to_file(FILE *file, TokenTable *table);

#endif //TOKEN_TABLE_H
