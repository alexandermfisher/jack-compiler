#ifndef TOKEN_TABLE_H
#define TOKEN_TABLE_H

#include <stdbool.h>
#include <stdio.h>

// Opaque TokenTable type
typedef struct TokenTable TokenTable;

// Function pointer types for handling generic tokens
typedef void (*TokenFreeFunc)(void *token);
typedef char* (*TokenToStr)(void *token);

/**
 * Creates a new, empty TokenTable instance.
 *
 * @param free_func Function pointer for freeing tokens (user must provide).
 * @param token_to_str Function pointer for converting token to string (optional, can be NULL).
 * @return Pointer to the new TokenTable. Caller must free it with token_table_free().
 */
TokenTable *token_table_create(TokenFreeFunc free_func, TokenToStr token_to_str);

/**
 * Adds a token to the TokenTable (ownership is transferred).
 *
 * @param table Pointer to the TokenTable.
 * @param token Pointer to the token data to add.
 * @return true if successful, false otherwise.
 */
bool token_table_add(TokenTable *table, void *token);

/**
 * Retrieves the next token and advances the iterator.
 *
 * @param table Pointer to the TokenTable.
 * @return Pointer to the next token, or NULL if done.
 */
void *token_table_next(TokenTable *table);

/**
 * Peeks at the next token without advancing the iterator.
 *
 * @param table Pointer to the TokenTable.
 * @return Pointer to the next token, or NULL if done.
 */
void *token_table_peek(TokenTable *table);

/**
 * Resets the internal iterator.
 *
 * @param table Pointer to the TokenTable.
 */
void token_table_reset(TokenTable *table);

/**
 * Frees the TokenTable and all contained tokens using the user-provided free function.
 *
 * @param table Pointer to the TokenTable.
 */
void token_table_free(TokenTable *table);

/**
 * Writes all tokens in the TokenTable using the user-provided print function.
 *
 * @param file Output stream.
 * @param table TokenTable to print.
 */
void token_table_write_to_file(FILE *file, TokenTable *table);

#endif //TOKEN_TABLE_H
