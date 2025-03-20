#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "token_table.h"

// Example generic "token" for testing
typedef struct {
    char *value;
} MockToken;

// Free function matching TokenFreeFunc
void mock_token_free(void *token) {
    MockToken *mt = (MockToken *)token;
    free(mt->value);
    free(mt);
}

// To-string function matching TokenToStr
char *mock_token_to_str(void *token) {
    MockToken *mt = (MockToken *)token;
    char *str = malloc(strlen(mt->value) + 1);
    if (str) strcpy(str, mt->value);
    return str;
}

// Helper to create a MockToken
MockToken *create_mock_token(const char *value) {
    MockToken *mt = malloc(sizeof(MockToken));
    if (!mt) return NULL;
    mt->value = strdup(value);
    if (!mt->value) {
        free(mt);
        return NULL;
    }
    return mt;
}

void test_token_table(void);

int main(void) {
    test_token_table();
    return 0;
}

void test_token_table(void) {
    TokenTable *table = token_table_create(mock_token_free, mock_token_to_str);
    assert(table != NULL);

    // Create mock tokens
    MockToken *token1 = create_mock_token("FIRST");
    MockToken *token2 = create_mock_token("SECOND");
    MockToken *token3 = create_mock_token("THIRD");

    assert(token1 && token2 && token3);

    // Add tokens to the table
    assert(token_table_add(table, token1));
    assert(token_table_add(table, token2));
    assert(token_table_add(table, token3));

    // Verify retrieval order
    token_table_reset(table);
    void *retrieved = token_table_next(table);
    assert(retrieved == token1);

    retrieved = token_table_next(table);
    assert(retrieved == token2);

    retrieved = token_table_next(table);
    assert(retrieved == token3);

    assert(token_table_next(table) == NULL);

    // Test writing to file (stdout here)
//    printf("Tokens in table:\n");
//    token_table_write_to_file(stdout, table);

    // Clean up
    token_table_free(table);

    printf("\t✅ test_token_table passed!\n");
}
