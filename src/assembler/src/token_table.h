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

/**
 * Creates a new, empty TokenTable instance.
 *
 * @return Pointer to the newly created TokenTable. Caller is responsible for freeing it with token_table_free().
 */
TokenTable *token_table_create(void);

/**
 * Adds a Token to the TokenTable.
 *
 * @param table Pointer to the TokenTable.
 * @param token Pointer to the Token to add. Ownership is transferred to the table.
 * @return true if the token was successfully added, false otherwise (e.g., on memory error).
 */
bool token_table_add(TokenTable *table, Token *token);

/**
 * Retrieves the next Token from the TokenTable and advances the internal iterator.
 *
 * @param table Pointer to the TokenTable.
 * @return Pointer to the next Token, or NULL if no more tokens remain.
 */
Token *token_table_next(TokenTable *table);

/**
 * Peeks at the next Token in the TokenTable without advancing the iterator.
 *
 * @param table Pointer to the TokenTable.
 * @return Pointer to the next Token, or NULL if no more tokens remain.
 */
Token *token_table_peek(TokenTable *table);

/**
 * Resets the internal iterator of the TokenTable to the start.
 *
 * @param table Pointer to the TokenTable.
 */
void token_table_reset(TokenTable *table);

/**
 * Frees the memory used by the TokenTable, including all stored Tokens.
 *
 * @param table Pointer to the TokenTable to free.
 */
void token_table_free(TokenTable *table);

/**
 * Writes all Tokens in the TokenTable to the specified file in a human-readable format.
 *
 * @param file Pointer to the output FILE stream (must be opened by caller).
 * @param table Pointer to the TokenTable whose tokens will be written.
 */
void token_table_write_to_file(FILE *file, TokenTable *table);

#endif //TOKEN_TABLE_H
