//
// Created by alexanderfisher on 09/03/25.
//

#include <assert.h>
#include <string.h>
#include <token.h>

void test_create_token(void);
void test_add_token(void);

int main(void) {
    printf("Starting test_token...\n");
    test_create_token();
    test_add_token();
    printf("All tests passed successfully.\n");
    printf("Test completed.\n");
    return 0;
}

void test_create_token(void) {
    // Test SYMBOL token
    Token *symbol_token = create_token(SYMBOL, "LOOP");
    assert(symbol_token != NULL);
    assert(symbol_token->type == SYMBOL);
    assert(symbol_token->value.symbol != NULL);
    assert(strcmp(symbol_token->value.symbol, "LOOP") == 0);
    free_token(symbol_token);

    // Test INTEGER_LITERAL token
    int num = 42;
    Token *int_token = create_token(INTEGER_LITERAL, &num);
    assert(int_token != NULL);
    assert(int_token->type == INTEGER_LITERAL);
    assert(int_token->value.integer == 42);
    free_token(int_token);

    // Test KEYWORD token
    Keyword kw = KW_JMP;
    Token *kw_token = create_token(KEYWORD, &kw);
    assert(kw_token != NULL);
    assert(kw_token->type == KEYWORD);
    assert(kw_token->value.keyword == KW_JMP);
    free_token(kw_token);

    // Test OPERATOR token
    Operator op = OP_ADD;
    Token *op_token = create_token(OPERATOR, &op);
    assert(op_token != NULL);
    assert(op_token->type == OPERATOR);
    assert(op_token->value.operator == OP_ADD);
    free_token(op_token);

    // Test SEPARATOR token
    Separator sep = SEP_SEMICOLON;
    Token *sep_token = create_token(SEPARATOR, &sep);
    assert(sep_token != NULL);
    assert(sep_token->type == SEPARATOR);
    assert(sep_token->value.separator == SEP_SEMICOLON);
    free_token(sep_token);

    // Test NEWLINE token (should accept NULL value)
    Token *newline_token = create_token(NEWLINE, NULL);
    assert(newline_token != NULL);
    assert(newline_token->type == NEWLINE);
    free_token(newline_token);

    // Test INVALID token (should accept NULL value)
    Token *invalid_token = create_token(INVALID, NULL);
    assert(invalid_token != NULL);
    assert(invalid_token->type == INVALID);
    free_token(invalid_token);

    // Test NULL input for required value (should return NULL)
    assert(create_token(INTEGER_LITERAL, NULL) == NULL);
    assert(create_token(KEYWORD, NULL) == NULL);
    assert(create_token(OPERATOR, NULL) == NULL);
    assert(create_token(SEPARATOR, NULL) == NULL);

    printf("    ✅ test_create_token passed!\n");
}

void test_add_token(void) {
    TokenTable *head = NULL;
    TokenTable *tail = NULL;

    // Create tokens
    Token *token1 = create_token(SYMBOL, "FIRST");
    Token *token2 = create_token(SYMBOL, "SECOND");
    Token *token3 = create_token(SYMBOL, "THIRD");

    // Add tokens to the list
    assert(add_token(&head, &tail, token1) == true);
    assert(add_token(&head, &tail, token2) == true);
    assert(add_token(&head, &tail, token3) == true);

    // Verify head and tail
    assert(head != NULL);
    assert(tail != NULL);
    assert(head->token == token1);
    assert(tail->token == token3);

    // Verify list order by traversing
    const TokenTable *current = head;
    assert(current != NULL && current->token == token1);
    current = current->next;
    assert(current != NULL && current->token == token2);
    current = current->next;
    assert(current != NULL && current->token == token3);
    assert(current->next == NULL);  // Last token should have NULL next pointer

    free_token_table(&head);
    printf("    ✅ test_add_token passed!\n");
}