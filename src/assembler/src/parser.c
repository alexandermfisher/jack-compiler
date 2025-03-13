#include "parser.h"

#include <stdlib.h>

#define MAX_TOKENS_PER_INSTRUCTION 10

Parser *parser_create(TokenTable *token_table, SymbolTable *symbol_table) {
    if (!token_table || !symbol_table) return NULL;
    Parser *parser = malloc(sizeof(Parser));
    if (!parser) return NULL;

    parser->token_table = token_table;
    parser->symbol_table = symbol_table;
    parser->instruction = NULL;

    return parser;
}

void parser_free(Parser *parser) {
    if (!parser) return;

    if (parser->instruction) {
        free(parser->instruction);
    }
    free(parser);
}

bool parser_has_more_commands(Parser *parser) {
    if (!parser || !parser->token_table || !token_table_peek(parser->token_table)) {
        return false;
    }
    return true;
}

bool advance(Parser *parser) {
    if (!parser) return false;

    // Free current instruction:
    if (parser->instruction) {
        free(parser->instruction);
        parser->instruction = NULL;
    }

    // Read tokens into buffer until newline token or NULL:
    Token *tokens[MAX_TOKENS_PER_INSTRUCTION];
    int token_count = 0;
    do {
        if (token_count == MAX_TOKENS_PER_INSTRUCTION) return false;

        tokens[token_count++] = token_table_next(parser->token_table);
    } while (!tokens[token_count - 1] || tokens[token_count - 1]->type != NEWLINE);

    static int instruction_count = 1;
    printf("Instruction %d:\n", instruction_count);
    for (int i = 0; i < token_count; i++) {
        char *token_str = token_to_str(tokens[i]);
        printf("\t\t%s\n", token_str);
        free(token_str);
    }
    printf("\n");
    instruction_count++;

    Instruction *instruction = malloc(sizeof(Instruction));
    if (!instruction) {
        return NULL;
    }
    int current_inst = 0;

    // Parse L-instruction:
    if (tokens[current_inst]->type == SEPARATOR &&
        tokens[current_inst++]->value.separator == SEP_LPAREN) {

        instruction->type = L_INSTRUCTION;

        // Process either symbol or integer literal:
        if (tokens[current_inst]->type != SYMBOL) {
            free(instruction);
            return false;
        }

        instruction->symbol = tokens[current_inst++]->value.symbol;

        if (tokens[current_inst]->type == SEPARATOR &&
        tokens[current_inst++]->value.separator == SEP_RPAREN) {
            free(instruction);
            return false;
        }

        // Process Newline:
    }

    // Parse A-instruction:


    return true;
}









