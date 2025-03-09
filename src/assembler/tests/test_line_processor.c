//
// Created by Alexander Fisher on 09/03/2025.
//
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <token_table.h>
#include <line_processor.h>

void test_process_line(void);
void test_preprocess_line(void);
void test_process_a_instruction(void);

int main(void) {
    printf("Starting test_assembler...\n");
    test_preprocess_line();
    test_process_a_instruction();
    //test_process_line();
    printf("Test completed.\n");
    return 0;
}

void test_process_line(void) {
    printf("Running test_process_line...\n");

    // Create a token table
    TokenTable *table = token_table_create();
    assert(table != NULL);

    Token *token;

    // @2 -> OPERATOR OP_AT, INTEGER_LITERAL 2
    assert(process_line("@2", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 2);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // D=A -> KEYWORD KW_D, OPERATOR OP_ASSIGN, KEYWORD KW_A
    table = token_table_create();
    assert(process_line("D=A", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // D=D+A -> KEYWORD KW_D, OPERATOR OP_ASSIGN, KEYWORD KW_D, OPERATOR OP_ADD, KEYWORD KW_A
    table = token_table_create();
    assert(process_line("D=D+A", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ADD);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // (LOOP) -> SEPARATOR SEP_LPAREN, SYMBOL "LOOP", SEPARATOR SEP_RPAREN
    table = token_table_create();
    assert(process_line("(LOOP)", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LOOP") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // @(JMP) -> OPERATOR OP_AT, SEPARATOR SEP_LPAREN, KEYWORD KW_JMP, SEPARATOR SEP_RPAREN
    table = token_table_create();
    assert(process_line("@(JMP)", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_JMP);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // M=1+; -> KEYWORD KW_M, OPERATOR OP_ASSIGN, INTEGER_LITERAL 1, OPERATOR OP_ADD, SEPARATOR SEP_SEMICOLON
    table = token_table_create();
    assert(process_line("M=1+;", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_M);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 1);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ADD);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_SEMICOLON);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // (FIRST)SECOND -> SEPARATOR SEP_LPAREN, SYMBOL "FIRST", SEPARATOR SEP_RPAREN, SYMBOL "SECOND"
    table = token_table_create();
    assert(process_line("(FIRST)SECOND", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "FIRST") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "SECOND") == 0);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // (WRONG$) -> SEPARATOR SEP_LPAREN, INVALID "WRONG$", SEPARATOR SEP_RPAREN
    table = token_table_create();
    assert(process_line("(WRONG$)", table));
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
    token = token_table_next(table);
    assert(token != NULL && token->type == INVALID && strcmp(token->value.symbol, "WRONG$") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    printf("✅ test_process_line passed!\n");
}

void test_preprocess_line(void) {
    const struct {
        const char *input;
        const char *expected_output;
    } test_cases[] = {
        {"     D=M     // this is comment.", "D=M     // this is comment."},
        {"   // This whole line is a comment", NULL},
        {"A=D+M  ", "A=D+M  "},
        {"      ", NULL},
        {"//OnlyComment", NULL},
        {"@123  // Load 123", "@123  // Load 123"}
    };

    printf("Running test_preprocess_line...\n");

    const size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    for (size_t i = 0; i < num_tests; i++) {
        const char *processed = preprocess_line(test_cases[i].input);
        if (test_cases[i].expected_output == NULL) {
            assert(processed == NULL);  // Expecting NULL for ignored lines
        } else {
            assert(processed != NULL);  // Ensure we didn't get NULL
            assert(strcmp(processed, test_cases[i].expected_output) == 0);  // Compare processed output
        }
    }

    printf("    ✅ All tests passed!\n");
}

void test_process_a_instruction(void) {
    TokenTable *table = token_table_create();
    Token *token;

    printf("Running test_process_a_instruction...\n");

    // ✅ Test: @2 -> OPERATOR OP_AT, INTEGER_LITERAL 2
    assert(process_line("@2", table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 2);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @32767 (max Hack value)
    table = token_table_create();
    assert(process_line("@32767", table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 32767);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @LOOP (symbol)
    table = token_table_create();
    assert(process_line("@LOOP", table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LOOP") == 0);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @LABEL1 with comment
    table = token_table_create();
    assert(process_line("@LABEL1 // comment", table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LABEL1") == 0);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @ (missing value)
    table = token_table_create();
    assert(process_line("@", table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @5ABC (invalid integer)
    table = token_table_create();
    assert(process_line("@5ABC", table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @32768 (out of range)
    table = token_table_create();
    assert(process_line("@32768", table) == PROCESS_INVALID);  // Beyond Hack's 15-bit limit
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @5/ (invalid character after number)
    table = token_table_create();
    assert(process_line("@5/", table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    printf("    ✅ test_process_a_instruction passed!\n");
}

