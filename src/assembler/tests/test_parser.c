//
// Created by Alexander Fisher on 04/03/2025.
//
#include <assert.h>
#include <stdio.h>
#include <symbol_table.h>
#include <token_table.h>

void test_parser(void) {
    // Add your test logic here
}

int main(void) {
    test_parser();
    return 0;
}

void test_get_next_instruction() {
    TokenTable *table;
    SymbolTable *symbol_table = symbol_table_create();
    int rom_address;
    Token *tokens[MAX_TOKENS_PER_INSTRUCTION];
    Parser parser;

    // === TEST 1: Valid instruction (A-instruction) ===
    table = token_table_create();
    parser.token_table = table;
    parser.symbol_table = symbol_table;

    rom_address = 0;
    assert(process_line("@2", table, symbol_table, &rom_address) == PROCESS_SUCCESS);

    token_table_reset(table);
    int count = get_next_instruction(&parser, tokens);

    assert(count == 3);  // OP_AT, INTEGER_LITERAL, NEWLINE
    assert(tokens[0] != NULL && tokens[0]->type == OPERATOR && tokens[0]->value.operator == OP_AT);
    assert(tokens[1] != NULL && tokens[1]->type == INTEGER_LITERAL && tokens[1]->value.integer == 2);
    assert(tokens[2] != NULL && tokens[2]->type == NEWLINE);

    token_table_free(table);

    // === TEST 2: Valid instruction (C-instruction) ===
    table = token_table_create();
    parser.token_table = table;

    rom_address = 0;
    assert(process_line("D=M", table, symbol_table, &rom_address) == PROCESS_SUCCESS);

    token_table_reset(table);
    count = get_next_instruction(&parser, tokens);

    assert(count == 4);  // KW_D, OP_ASSIGN, KW_M, NEWLINE
    assert(tokens[0] != NULL && tokens[0]->type == KEYWORD && tokens[0]->value.keyword == KW_D);
    assert(tokens[1] != NULL && tokens[1]->type == OPERATOR && tokens[1]->value.operator == OP_ASSIGN);
    assert(tokens[2] != NULL && tokens[2]->type == KEYWORD && tokens[2]->value.keyword == KW_M);
    assert(tokens[3] != NULL && tokens[3]->type == NEWLINE);

    token_table_free(table);

    // === TEST 3: No NEWLINE (should return 0) ===
    table = token_table_create();
    parser.token_table = table;

    rom_address = 0;
    assert(process_line("D=M // Missing newline", table, symbol_table, &rom_address) == PROCESS_SUCCESS);

    token_table_reset(table);
    count = get_next_instruction(&parser, tokens);

    assert(count == 0);  // No NEWLINE means instruction parsing is incomplete
    assert(parser.instruction == NULL);

    token_table_free(table);

    // === TEST 4: Too many tokens (exceeds MAX_TOKENS_PER_INSTRUCTION) ===
    table = token_table_create();
    parser.token_table = table;

    rom_address = 0;
    assert(process_line("A=M+1;JGT // Excessive tokens", table, symbol_table, &rom_address) == PROCESS_SUCCESS);

    token_table_reset(table);
    count = get_next_instruction(&parser, tokens);

    assert(count == 0);  // Too many tokens should trigger failure
    assert(parser.instruction == NULL);

    token_table_free(table);

    symbol_table_free(symbol_table);
    printf("All tests passed!\n");
}

