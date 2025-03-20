//
// Created by Alexander Fisher on 04/03/2025.
//
#include <assert.h>
#include <parser.h>
#include <symbol_table.h>
#include <token.h>
#include <token_table.h>
#include <lexer.h>
#include <instruction.h>
#include <string.h>
#include <stdio.h>

void test_parser(void);

int main(void) {
    test_parser();
    return 0;
}

void test_parser(void) {
    SymbolTable *symbol_table = symbol_table_create();
    int rom_address = 0;
    TokenTable *table = token_table_create((TokenFreeFunc)free_token, (TokenToStr)token_to_str);

    Parser *parser = parser_create(table, symbol_table);

    // === Input program with all 3 instruction types ===
    char *program[] = {
        "@21",
        "D=M",
        "(LOOP)",
        "D;JGT",
        NULL
    };

    // Lex all lines first
    for (int i = 0; program[i] != NULL; i++) {
        assert(lex_line(program[i], strlen(program[i]), table, symbol_table, &rom_address) == PROCESS_SUCCESS);
    }

    token_table_reset(table);

    // === Parse and check A-instruction ===
    assert(parser_has_more_commands(parser));
    assert(advance(parser));
    assert(parser->instruction->type == A_INSTRUCTION_VALUE);
    assert(parser->instruction->value == 21);

    // === Parse and check C-instruction ===
    assert(parser_has_more_commands(parser));
    assert(advance(parser));
    assert(parser->instruction->type == C_INSTRUCTION);
    assert(parser->instruction->dest == TOKEN_DEST_D);
    assert(parser->instruction->comp == TOKEN_COMP_M);
    assert(parser->instruction->jump == TOKEN_JUMP_NULL);

    // === Parse and check L-instruction (label) ===
    assert(parser_has_more_commands(parser));
    assert(advance(parser));
    assert(parser->instruction->type == L_INSTRUCTION);
    assert(strcmp(parser->instruction->symbol, "LOOP") == 0);

    // === Parse and check C-instruction with jump ===
    assert(parser_has_more_commands(parser));
    assert(advance(parser));
    assert(parser->instruction->type == C_INSTRUCTION);
    assert(parser->instruction->dest == TOKEN_DEST_NULL);
    assert(parser->instruction->comp == TOKEN_COMP_D);
    assert(parser->instruction->jump == TOKEN_JUMP_JGT);

    // === No more instructions ===
    assert(!parser_has_more_commands(parser));

    // Cleanup
    parser_free(parser);
    token_table_free(table);
    symbol_table_free(symbol_table);
    printf("\t✅ test_parser passed!\n");
}
