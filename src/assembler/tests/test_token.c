//
// Created by alexanderfisher on 09/03/25.
//

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <token.h>
#include <stdio.h>

void test_create_token(void);

int main(void) {
    test_create_token();
    return 0;
}

void test_create_token(void) {
    // Test SYMBOL token
    Token *symbol_token = create_token(TOKEN_SYMBOL, "LOOP");
    assert(symbol_token != NULL);
    assert(symbol_token->type == TOKEN_SYMBOL);
    assert(symbol_token->value.symbol != NULL);
    assert(strcmp(symbol_token->value.symbol, "LOOP") == 0);
    free_token(symbol_token);

    // Edge case: Empty string for SYMBOL
    Token *empty_symbol_token = create_token(TOKEN_SYMBOL, "");
    assert(empty_symbol_token != NULL);
    assert(empty_symbol_token->type == TOKEN_SYMBOL);
    assert(empty_symbol_token->value.symbol != NULL);
    assert(strcmp(empty_symbol_token->value.symbol, "") == 0);
    free_token(empty_symbol_token);

    // Test INTEGER_LITERAL token
    Token *int_token = create_token(TOKEN_INTEGER, 42);
    assert(int_token != NULL);
    assert(int_token->type == TOKEN_INTEGER);
    assert(int_token->value.integer == 42);
    free_token(int_token);

    // Edge case: Large and small integers
    Token *large_int_token = create_token(TOKEN_INTEGER, INT_MAX);
    assert(large_int_token != NULL);
    assert(large_int_token->value.integer == INT_MAX);
    free_token(large_int_token);

    Token *small_int_token = create_token(TOKEN_INTEGER, INT_MIN);
    assert(small_int_token != NULL);
    assert(small_int_token->value.integer == INT_MIN);
    free_token(small_int_token);

    // Test KEYWORD token
    Token *kw_token = create_token(TOKEN_JUMP_JMP);
    assert(kw_token != NULL);
    assert(kw_token->type == TOKEN_JUMP_JMP);
    free_token(kw_token);

    // Test NEWLINE token
    Token *newline_token = create_token(NEWLINE);
    assert(newline_token != NULL);
    assert(newline_token->type == NEWLINE);
    assert(newline_token->value.symbol == NULL);
    free_token(newline_token);

    // Edge case: NEWLINE with unexpected value (should still be NULL)
    Token *unexpected_newline = create_token(NEWLINE, "unexpected");
    assert(unexpected_newline != NULL);
    assert(unexpected_newline->type == NEWLINE);
    assert(unexpected_newline->value.symbol == NULL);
    free_token(unexpected_newline);

    printf("\t✅ test_create_token passed!\n");
}
