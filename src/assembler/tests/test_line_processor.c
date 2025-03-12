//
// Created by Alexander Fisher on 09/03/2025.
//
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <token_table.h>
#include <line_processor.h>
#include <symbol_table.h>
#include <unistd.h>
#include <config.h>


void test_preprocess_line(void);
void test_is_keyword(void);
void test_is_line_end_or_comment(void);
void test_process_symbol(void);
void test_process_label(void);
void test_process_integer_literal(void);
void test_process_a_instruction(void);
void test_process_c_instruction(void);

void test_process_line(void);

// Helper macro to simplify token checks
#define CHECK_TOKEN(token, expected_type, expected_value) \
assert(token != NULL && token->type == expected_type && token->value.keyword == expected_value);


int main(void) {
    test_preprocess_line();
    test_is_keyword();
    test_is_line_end_or_comment();
    test_process_symbol();
    test_process_label();
    test_process_label();
    test_process_integer_literal();
    test_process_a_instruction();
    test_process_c_instruction();
    //test_process_line();


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

    printf("\t✅ test_preprocess_line passed!\n");
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

    printf("\t✅ test_is_keyword passed!\n");
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

    printf("\t✅ test_is_line_end_or_comment passed!\n");
}

void test_process_symbol(void) {

    const char *line = NULL;
    char buffer[MAX_LABEL_LEN + 1];

    // ✅ Test: Valid symbol "LOOPrest"
    line = "LOOP rest";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "LOOPrest") == 0);
    assert(strcmp(line, "\0") == 0);

    // ✅ Test: Valid symbol with special characters "_my.label$"
    line = "_my.label$ more";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "_my.label$more") == 0);
    assert(strcmp(line, "\0") == 0);

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
    assert(strcmp(line, "\0") == 0);

    // ✅ Test: Symbol can start with leading space
    line = "   varName";
    assert(process_symbol(&line, buffer) == PROCESS_SUCCESS);
    assert(strcmp(buffer, "varName") == 0);
    assert(strcmp(line, "\0") == 0);

    // ❌ Test: Symbol cannot start with a digit
    line = "123abc";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Symbol cannot start with a hyphen
    line = "-LABEL";
    assert(process_symbol(&line, buffer) == PROCESS_INVALID);

    // ❌ Test: Symbol cannot start with an operator
    line = "+variable";
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
    printf("\t✅test_process_symbol passed!\n");
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

    // ✅ Test: Label with spaces before and after
    const char *line12 = "( JUMP )";
    assert(process_label(&line12, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "JUMP"));
    assert(symbol_table_get_address(symbol_table, "JUMP") == 4);
    assert(rom_address == 5);

    // ❌ Test: Label with leading spaces
    const char *line13 = "   (LABEL)";
    assert(process_label(&line13, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Label with spaces before and after the parentheses
    const char *line14 = "   (LABEL)   ";
    assert(process_label(&line14, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ✅ Test: Label with spaces in the middle (multiple words should be treated as one)
    const char *line15 = "(MULTI WORD)";
    assert(process_label(&line15, token_table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    assert(symbol_table_contains(symbol_table, "MULTIWORD"));  // Spaces should be ignored
    assert(symbol_table_get_address(symbol_table, "MULTIWORD") == 5);
    assert(rom_address == 6);

    // ❌ Test: Label cannot start with a digit
    const char *line16 = "(9symbol)";
    assert(process_label(&line16, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Invalid character (#) in label
    const char *line17 = "(#invalid)";
    assert(process_label(&line17, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Multiple parentheses (invalid double label declaration)
    const char *line18 = "(A)(B)";
    assert(process_label(&line18, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Empty label (nothing between parentheses)
    const char *line19 = "()";
    assert(process_label(&line19, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Labels cannot contain a decimal point
    const char *line20 = "(1.23)";
    assert(process_label(&line20, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // ❌ Test: Invalid label containing special characters (_#)
    const char *line21 = "(_#notAllowed)";
    assert(process_label(&line21, token_table, symbol_table, &rom_address) == PROCESS_INVALID);

    // Cleanup
    symbol_table_free(symbol_table);
    token_table_free(token_table);

    printf("\t✅ test_process_label passed!\n");
}

void test_process_integer_literal(void) {
    int result;

    // ✅ Test: Valid minimum value (0)
    const char *line1 = "0";
    assert(process_integer_literal(&line1, &result) == PROCESS_SUCCESS);
    assert(result == 0);

    // ✅ Test: Valid maximum 15-bit value (32767)
    const char *line2 = "32767";
    assert(process_integer_literal(&line2, &result) == PROCESS_SUCCESS);
    assert(result == 32767);

    // ❌ Test: Value exceeding 15-bit limit (32768)
    const char *line3 = "32768";
    assert(process_integer_literal(&line3, &result) == PROCESS_INVALID);

    // ❌ Test: Negative value (not allowed in Hack)
    const char *line4 = "-1";
    assert(process_integer_literal(&line4, &result) == PROCESS_INVALID);

    // ✅ Test: Valid number with trailing space
    const char *line5 = "123 ";
    assert(process_integer_literal(&line5, &result) == PROCESS_SUCCESS);
    assert(result == 123);

    // ✅ Test: Valid number with comment after
    const char *line6 = "456 // comment";
    assert(process_integer_literal(&line6, &result) == PROCESS_SUCCESS);
    assert(result == 456);

    // ❌ Test: Non-numeric input
    const char *line7 = "abc";
    assert(process_integer_literal(&line7, &result) == PROCESS_INVALID);

    // ❌ Test: Mixed numeric and non-numeric characters
    const char *line8 = "12a34";
    assert(process_integer_literal(&line8, &result) == PROCESS_INVALID);

    // ❌ Test: Empty string
    const char *line9 = "";
    assert(process_integer_literal(&line9, &result) == PROCESS_INVALID);

    // ❌ Test: NULL input
    assert(process_integer_literal(NULL, &result) == PROCESS_ERROR);

    // ❌ Test: NULL output pointer
    const char *line10 = "42";
    assert(process_integer_literal(&line10, NULL) == PROCESS_ERROR);

    printf("\t✅ test_process_integer_literal passed!\n");
}

void test_process_a_instruction(void) {
    TokenTable *table = token_table_create();

    // ✅ Test: @2 -> OPERATOR OP_AT, INTEGER_LITERAL 2
    const char *line1 = "@2";
    assert(process_a_instruction(&line1, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    const Token *token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 2);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @32767 (max Hack value)
    table = token_table_create();
    const char *line2 = "@32767";
    assert(process_a_instruction(&line2, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 32767);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @LOOP (symbol)
    table = token_table_create();
    const char *line3 = "@LOOP";
    assert(process_a_instruction(&line3, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LOOP") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @LABEL1 with comment
    table = token_table_create();
    const char *line4 = "@LABEL1 // comment";
    assert(process_a_instruction(&line4, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LABEL1") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @ (missing value)
    table = token_table_create();
    const char *line5 = "@";
    assert(process_a_instruction(&line5, table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @5ABC (invalid integer)
    table = token_table_create();
    const char *line6 = "@5ABC";
    assert(process_a_instruction(&line6, table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @32768 (out of range)
    table = token_table_create();
    const char *line7 = "@32768";
    assert(process_a_instruction(&line7, table) == PROCESS_INVALID);  // Beyond Hack's 15-bit limit
    assert(token_table_next(table) == NULL);
    token_table_free(table);


    // 🚫 Test: @5/ (invalid character after number)
    table = token_table_create();
    const char *line8 = "@5/";
    assert(process_a_instruction(&line8, table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @  123 (whitespace after @)
    table = token_table_create();
    const char *line9 = "@  123";
    assert(process_a_instruction(&line9, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 123);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @LABEL   (whitespace after symbol)
    table = token_table_create();
    const char *line10 = "@LABEL   ";
    assert(process_a_instruction(&line10, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LABEL") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @   LOOP (whitespace before symbol)
    table = token_table_create();
    const char *line11 = "@   LOOP";
    assert(process_a_instruction(&line11, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "LOOP") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: @MIX   ED_SYM (whitespace inside symbol)
    table = token_table_create();
    const char *line12 = "@MIX   ED_SYM";
    assert(process_a_instruction(&line12, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == SYMBOL && strcmp(token->value.symbol, "MIXED_SYM") == 0);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @   (only @ with spaces)
    table = token_table_create();
    const char *line13 = "@   ";
    assert(process_a_instruction(&line13, table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @ 5ABC (invalid integer, must not contain letters)
    table = token_table_create();
    const char *line14 = "@ 5ABC";
    assert(process_a_instruction(&line14, table) == PROCESS_INVALID);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // 🚫 Test: @  0123 (leading zero is invalid)
    table = token_table_create();
    const char *line15 = "@  0123";
    assert(process_a_instruction(&line15, table) == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_AT);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 123);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);


    printf("        ✅ test_process_a_instruction passed!\n");
}

void test_process_c_instruction(void) {
    TokenTable *table;
    ProcessStatus status;
    Token *token;

    // ✅ Test: Simple instruction "D=A"
    table = token_table_create();
    const char *line1 = "D=A";
    status = process_c_instruction(&line1, table);
    assert(status == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: Instruction with extra whitespace " D  =   M "
    table = token_table_create();
    const char *line2 = " D  =   M ";
    status = process_c_instruction(&line2, table);
    assert(status == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_M);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: C-instruction with computation "D=D+1"
    table = token_table_create();
    const char *line3 = "D=D+1";
    status = process_c_instruction(&line3, table);
    assert(status == PROCESS_SUCCESS);
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
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 1);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: C-instruction with jump "D;JGT"
    table = token_table_create();
    const char *line4 = "D;JGT";
    status = process_c_instruction(&line4, table);
    assert(status == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_SEMICOLON);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_JGT);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: C-instruction with computation and jump "A=D+1;JMP"
    table = token_table_create();
    const char *line5 = "A=D+1;JMP";
    status = process_c_instruction(&line5, table);
    assert(status == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ADD);
    token = token_table_next(table);
    assert(token != NULL && token->type == INTEGER_LITERAL && token->value.integer == 1);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_SEMICOLON);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_JMP);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ✅ Test: Instruction with extra spaces before and after "   M   =   A   ;   JNE   "
    table = token_table_create();
    const char *line6 = "   M   =   A   ;   JNE   ";
    status = process_c_instruction(&line6, table);
    assert(status == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_M);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_A);
    token = token_table_next(table);
    assert(token != NULL && token->type == SEPARATOR && token->value.separator == SEP_SEMICOLON);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_JNE);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    // ❌ Test: Invalid instruction "X=Y"
    table = token_table_create();
    const char *line7 = "X=Y";
    status = process_c_instruction(&line7, table);
    assert(status == PROCESS_INVALID);
    token_table_free(table);

    // ❌ Test: Invalid jump mnemonic "D;XYZ"
    table = token_table_create();
    const char *line8 = "D;XYZ";
    status = process_c_instruction(&line8, table);
    assert(status == PROCESS_INVALID);
    token_table_free(table);

    // ✅ Test: C-instruction with incomplete RHS "D="
    table = token_table_create();
    const char *line9 = "D=";
    status = process_c_instruction(&line9, table);
    assert(status == PROCESS_SUCCESS);
    token_table_reset(table);
    token = token_table_next(table);
    assert(token != NULL && token->type == KEYWORD && token->value.keyword == KW_D);
    token = token_table_next(table);
    assert(token != NULL && token->type == OPERATOR && token->value.operator == OP_ASSIGN);
    token = token_table_next(table);
    assert(token != NULL && token->type == NEWLINE);
    assert(token_table_next(table) == NULL);
    token_table_free(table);

    printf("        ✅ test_process_c_instruction passed!\n");
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

