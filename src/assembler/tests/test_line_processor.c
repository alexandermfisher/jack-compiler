//
// Created by Alexander Fisher on 09/03/2025.
//
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <token_table.h>
#include <line_processor.h>
#include <symbol_table.h>
#include <stdlib.h>
#include <unistd.h>
#include <config.h>


void test_preprocess_line(void);
void test_is_keyword(void);
void test_is_line_end_or_comment(void);
void test_process_symbol(void);
void test_process_label(void);


void test_process_line(void);
void test_process_a_instruction(void);


int main(void) {
    printf("Starting test_line_processor...\n");
    test_preprocess_line();
    test_is_keyword();
    test_is_line_end_or_comment();
    test_process_symbol();
    test_process_label();
    test_process_label();
    //test_process_a_instruction();
    //test_process_line();
    printf("Test completed.\n");


    return 0;
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

    const size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    for (size_t i = 0; i < num_tests; i++) {
        const char *processed = preprocess_line(test_cases[i].input);
        if (test_cases[i].expected_output == NULL) {
            assert(processed == NULL);
        } else {
            assert(processed != NULL);
            assert(strcmp(processed, test_cases[i].expected_output) == 0);
        }
    }

    printf("    ✅ test_preprocess_line passed!\n");
}

void test_is_keyword(void) {

    // ✅ Test: Reserved keywords
    assert(is_keyword("A"));
    assert(is_keyword("D"));
    assert(is_keyword("JMP"));
    assert(is_keyword("SCREEN"));
    assert(is_keyword("THIS"));

    // ❌ Test: Non-keywords
    assert(!is_keyword("LOOP"));
    assert(!is_keyword("MY_VAR"));
    assert(!is_keyword("symbol123"));
    assert(!is_keyword("customLabel"));

    printf("    ✅ test_is_keyword passed!\n");
}

void test_is_line_end_or_comment(void) {

    // ✅ Test: Only whitespace
    assert(is_line_end_or_comment("   \t  "));

    // ✅ Test: Comment only
    assert(is_line_end_or_comment("// this is a comment"));

    // ✅ Test: Whitespace + comment
    assert(is_line_end_or_comment("   // comment here"));

    // ❌ Test: Invalid characters after whitespace
    assert(!is_line_end_or_comment(" JMP // comment"));

    // ❌ Test: Just an invalid character
    assert(!is_line_end_or_comment("X"));

    printf("    ✅ test_is_line_end_or_comment passed!\n");
}

void test_process_symbol(void) {

    const char *line = NULL;
    char buffer[MAX_LABEL_LEN + 1];

    // ✅ Test: Valid symbol "LOOP"
    line = "LOOP rest";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "LOOP") == 0);
    assert(strcmp(line, " rest") == 0);  // Line pointer should now be at " rest"

    // ✅ Test: Valid symbol with special characters "_my.label$"
    line = "_my.label$ more";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "_my.label$") == 0);
    assert(strcmp(line, " more") == 0);

    // ✅ Test: Reads valid portion before encountering an invalid character ('@')
    line = "var@name";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "var") == 0);
    assert(strcmp(line, "@name") == 0);  // Line pointer should stop at '@'

    // ✅ Test: Reads valid portion before encountering an invalid character ('#')
    line = "label#test";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "label") == 0);
    assert(strcmp(line, "#test") == 0);

    // ✅ Test: Reads valid portion before encountering whitespace
    line = "Symbol1 ";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "Symbol1") == 0);
    assert(strcmp(line, " ") == 0);

    // ❌ Test: Symbol cannot start with a digit
    line = "123abc";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Symbol cannot start with a hyphen
    line = "-LABEL";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Symbol cannot start with an operator
    line = "+variable";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Symbol cannot start with a space
    line = " varName";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Symbol cannot start with a parenthesis
    line = "(label)";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Empty string should be invalid
    line = "";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ✅ Test: Max length symbol
    char long_symbol[MAX_LABEL_LEN + 2];
    memset(long_symbol, 'A', MAX_LABEL_LEN);
    long_symbol[MAX_LABEL_LEN] = 'B'; // Will be truncated off buffer
    long_symbol[MAX_LABEL_LEN + 1] = '\0';
    line = long_symbol;
    memset(buffer, 0, MAX_LABEL_LEN + 1);
    assert(process_symbol(&line, buffer) == PROCESS_ERROR);
    assert(strlen(buffer) == MAX_LABEL_LEN);
    printf("    ✅ test_process_symbol passed!\n");
}

void test_process_label(void) {

    SymbolTable *symbol_table = symbol_table_create();
    TokenTable *token_table = token_table_create();
    int rom_address = 0;

    assert(symbol_table != NULL);
    assert(token_table != NULL);

    // ✅ Test: Valid label
    const char *line1 = "(LOOP)";
    assert(process_label(&line1, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "LOOP"));
    assert(symbol_table_get_address(symbol_table, "LOOP") == 0);
    assert(rom_address == 1);

    // ✅ Test: Valid label with new address
    const char *line2 = "(END)";
    assert(process_label(&line2, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "END"));
    assert(symbol_table_get_address(symbol_table, "END") == 1);
    assert(rom_address == 2);

    // ❌ Test: Reserved keyword as label
    const char *line3 = "(THIS)";
    assert(process_label(&line3, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Missing '('
    const char *line4 = "LOOP)";
    assert(process_label(&line4, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Empty label
    const char *line5 = "()";
    assert(process_label(&line5, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Duplicate label
    const char *line6 = "(LOOP)";
    assert(process_label(&line6, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_get_address(symbol_table, "LOOP") == 0);  // Should remain at 0
    assert(rom_address == 2);  // Should not increment again

    // ❌ Test: Extra closing parenthesis
    const char *line7 = "(LABEL))";
    assert(process_label(&line7, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Closing parenthesis followed by a slash
    const char *line8 = "(LABEL)/";
    assert(process_label(&line8, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ✅  Test: Closing parenthesis immediately followed by double slashes
    const char *line9 = "(LABEL)//";
    assert(process_label(&line9, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "LABEL"));
    assert(symbol_table_get_address(symbol_table, "LABEL") == 2);
    assert(rom_address == 3);

    // ✅ Test: Closing parenthesis followed by a comment (valid)
    const char *line10 = "(jump_10) // comment";
    assert(process_label(&line10, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "jump_10"));
    assert(symbol_table_get_address(symbol_table, "jump_10") == 3);
    assert(rom_address == 4);

    // ✅ Test: Label with trailing spaces
    const char *line11 = "(LABEL)   ";
    assert(process_label(&line11, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "LABEL"));
    assert(symbol_table_get_address(symbol_table, "LABEL") == 2);
    assert(rom_address == 4);

    // ❌ Test: Label with spaces before and after
    const char *line12 = "( JUMP )";
    assert(process_label(&line12, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Label with leading spaces
    const char *line13 = "   (LABEL)";
    assert(process_label(&line13, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Label with spaces before and after the parentheses
    const char *line14 = "   (LABEL)   ";
    assert(process_label(&line14, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Label with spaces in the middle (multiple words)
    const char *line15 = "(MULTI WORD)";
    assert(process_label(&line15, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // Cleanup
    symbol_table_free(symbol_table);
    token_table_free(token_table);

    printf("    ✅ test_process_label passed!\n");
}







//
// void test_process_line(void) {
//     printf("Running test_process_line...\n");
//
//     // Create a token table
//     TokenTable *table = token_table_create();
//     assert(table != NULL);
//
//     Token *token;
//
//     // @2 -> OPERATOR OP_AT, INTEGER_LITERAL 2
//     assert(process_line("@2", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 2);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // D=A -> KEYWORD KW_D, OPERATOR OP_ASSIGN, KEYWORD KW_A
//     table = token_table_create();
//     assert(process_line("D=A", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // D=D+A -> KEYWORD KW_D, OPERATOR OP_ASSIGN, KEYWORD KW_D, OPERATOR OP_ADD, KEYWORD KW_A
//     table = token_table_create();
//     assert(process_line("D=D+A", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ADD);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // (LOOP) -> SEPARATOR SEP_LPAREN, SYMBOL "LOOP", SEPARATOR SEP_RPAREN
//     table = token_table_create();
//     assert(process_line("(LOOP)", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LOOP") == 0);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // @(JMP) -> OPERATOR OP_AT, SEPARATOR SEP_LPAREN, KEYWORD KW_JMP, SEPARATOR SEP_RPAREN
//     table = token_table_create();
//     assert(process_line("@(JMP)", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_JMP);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // M=1+; -> KEYWORD KW_M, OPERATOR OP_ASSIGN, INTEGER_LITERAL 1, OPERATOR OP_ADD, SEPARATOR SEP_SEMICOLON
//     table = token_table_create();
//     assert(process_line("M=1+;", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_M);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 1);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ADD);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_SEMICOLON);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // (FIRST)SECOND -> SEPARATOR SEP_LPAREN, SYMBOL "FIRST", SEPARATOR SEP_RPAREN, SYMBOL "SECOND"
//     table = token_table_create();
//     assert(process_line("(FIRST)SECOND", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "FIRST") == 0);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "SECOND") == 0);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // (WRONG$) -> SEPARATOR SEP_LPAREN, INVALID "WRONG$", SEPARATOR SEP_RPAREN
//     table = token_table_create();
//     assert(process_line("(WRONG$)", table));
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_LPAREN);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == INVALID && strcmp(token->value.symbol, "WRONG$") == 0);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_RPAREN);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     printf("✅ test_process_line passed!\n");
// }
//
// void test_process_a_instruction(void) {
//     TokenTable *table = token_table_create();
//     Token *token;
//
//     printf("Running test_process_a_instruction...\n");
//
//     // ✅ Test: @2 -> OPERATOR OP_AT, INTEGER_LITERAL 2
//     assert(process_line("@2", table) == PROCESS_SUCCESS);
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 2);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // ✅ Test: @32767 (max Hack value)
//     table = token_table_create();
//     assert(process_line("@32767", table) == PROCESS_SUCCESS);
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 32767);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // ✅ Test: @LOOP (symbol)
//     table = token_table_create();
//     assert(process_line("@LOOP", table) == PROCESS_SUCCESS);
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LOOP") == 0);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // ✅ Test: @LABEL1 with comment
//     table = token_table_create();
//     assert(process_line("@LABEL1 // comment", table) == PROCESS_SUCCESS);
//     token_table_reset(table);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
//     token = token_table_next(table);
//     assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LABEL1") == 0);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // 🚫 Test: @ (missing value)
//     table = token_table_create();
//     assert(process_line("@", table) == PROCESS_INVALID);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // 🚫 Test: @5ABC (invalid integer)
//     table = token_table_create();
//     assert(process_line("@5ABC", table) == PROCESS_INVALID);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // 🚫 Test: @32768 (out of range)
//     table = token_table_create();
//     assert(process_line("@32768", table) == PROCESS_INVALID);  // Beyond Hack's 15-bit limit
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     // 🚫 Test: @5/ (invalid character after number)
//     table = token_table_create();
//     assert(process_line("@5/", table) == PROCESS_INVALID);
//     assert(token_table_next(table) == NULL);
//     token_table_free(table);
//
//     printf("    ✅ test_process_a_instruction passed!\n");
// }
//
