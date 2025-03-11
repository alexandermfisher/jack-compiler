//
// Created by Alexander Fisher on 09/03/2025.
//


#include <token.h>
#include <token_table.h>
#include <assert.h>

void test_token_table(void);

int main(void) {
    test_token_table();
    return 0;
}

void test_token_table(void) {
    TokenTable *table = token_table_create();
    assert(table != NULL);

    // Create tokens
    Token *token1 = create_token(SYMBOL, "FIRST");
    Token *token2 = create_token(SYMBOL, "SECOND");
    Token *token3 = create_token(SYMBOL, "THIRD");

    // Ensure tokens are created successfully
    assert(token1 != NULL);
    assert(token2 != NULL);
    assert(token3 != NULL);

    // Add tokens to the table
    assert(token_table_add(table, token1) == true);
    assert(token_table_add(table, token2) == true);
    assert(token_table_add(table, token3) == true);

    // Verify retrieval order using iterator-style access
    token_table_reset(table);
    const Token *retrieved = token_table_next(table);
    assert(retrieved != NULL && retrieved == token1);

    retrieved = token_table_next(table);
    assert(retrieved != NULL && retrieved == token2);

    retrieved = token_table_next(table);
    assert(retrieved != NULL && retrieved == token3);

    // Ensure we reach the end
    assert(token_table_next(table) == NULL);

    // Reset and verify iteration works again
    token_table_reset(table);
    assert(token_table_next(table) == token1);

    // Cleanup
    token_table_free(table);

    printf("\t✅ test_token_table passed!\n");
}
