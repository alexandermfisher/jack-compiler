
#include "parser.h"
#include "token.h"
#include <stdlib.h>

#define MAX_TOKENS_PER_INSTRUCTION 10

bool parse_l_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]);
bool parse_a_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]);
bool parse_c_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]);

void print_l_instruction(const Instruction *instruction);
void print_a_instruction(const Instruction *instruction);
void print_c_instruction(const Instruction *instr);


Parser *parser_create(TokenTable *token_table, SymbolTable *symbol_table) {
    if (!token_table || !symbol_table) return NULL;
    Parser *parser = malloc(sizeof(Parser));
    if (!parser) return NULL;

    parser->token_table = token_table;
    parser->symbol_table = symbol_table;
    parser->instruction = malloc(sizeof(Instruction));
    if (!parser->instruction) return NULL;

    return parser;
}

void parser_free(Parser *parser) {
    if (!parser) return;
    free(parser->instruction);
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

    // Read tokens into buffer until newline token or NULL:
    Token *tokens[MAX_TOKENS_PER_INSTRUCTION] = {0};
    int token_count = 0;
    do {
        if (token_count == MAX_TOKENS_PER_INSTRUCTION) return false;

        tokens[token_count++] = token_table_next(parser->token_table);
    } while (!tokens[token_count - 1] || tokens[token_count - 1]->type != NEWLINE);

    // Parse L-instruction:
    if (parse_l_instruction(parser, tokens)) {
        //print_l_instruction(parser->instruction);
        return true;
    }

    // Parse A-instruction:
    if (parse_a_instruction(parser, tokens)) {
        //print_a_instruction(parser->instruction);
        return true;
    }

    if (parse_c_instruction(parser, tokens)) {
        //print_c_instruction(parser->instruction);
        return true;
    }

    return false;
}

bool parse_l_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]) {
    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;
    int current_token = 0;

    if (tokens[current_token++]->type != TOKEN_LPAREN) return false;

    instruction->type = L_INSTRUCTION;

    if (tokens[current_token]->type != TOKEN_SYMBOL) return false;
    instruction->symbol = tokens[current_token++]->value.symbol;

    if (tokens[current_token++]->type != TOKEN_RPAREN) return false;

    if (tokens[current_token]->type != NEWLINE) return false;

    return true;
}

bool parse_a_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]) {
    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;
    int current_token = 0;

    if (tokens[current_token++]->type != TOKEN_AT) return false;

    if (tokens[current_token]->type == TOKEN_INTEGER) {
        instruction->type = A_INSTRUCTION_VALUE;
        instruction->value = tokens[current_token++]->value.integer;
    } else if (tokens[current_token]->type == TOKEN_SYMBOL) {
        instruction->type = A_INSTRUCTION_SYMBOL;
        instruction->symbol = tokens[current_token++]->value.symbol;
    } else return false;

    if (tokens[current_token]->type != NEWLINE) return false;

    return true;
}

bool parse_c_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]) {
    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;

    // Default values
    instruction->type = C_INSTRUCTION;
    instruction->dest = tokens[0]->type;
    instruction->comp = tokens[1]->type;
    instruction->jump = tokens[2]->type;

    // Ensure instruction ends with NEWLINE
    if (!tokens[3] || tokens[3]->type != NEWLINE) {
        return false;
    }

    return true;
}


// Function to print L-instruction
void print_l_instruction(const Instruction *instruction) {
    if (instruction->type == L_INSTRUCTION) {
        printf("(L-INST) Symbol: %s\n", instruction->symbol);
    }
}

// Function to print A-instruction
void print_a_instruction(const Instruction *instruction) {
    if (instruction->type == A_INSTRUCTION_SYMBOL) {
        printf("(A-INST) Symbolic: @%s\n", instruction->symbol);
    }
    else {
        printf("(A-INST) Numeric: @%d\n", instruction->value);
    }
}

// Function to print C-instruction
void print_c_instruction(const Instruction *instr) {
    if (!instr) {
        printf("Invalid instruction (NULL)\n");
        return;
    }

    // Print DEST field
    printf("DEST: ");
    if (instr->dest == TOKEN_DEST_A) {
        printf("NULL");
    } else {
        if (instr->dest & TOKEN_DEST_A) printf("A");
        if (instr->dest & TOKEN_DEST_M) printf("M");
        if (instr->dest & TOKEN_DEST_D) printf("D");
    }
    printf("\n");

    // Print COMP field
    printf("COMP: ");
    switch (instr->comp) {
        case TOKEN_COMP_0: printf("0"); break;
        case TOKEN_COMP_1: printf("1"); break;
        case TOKEN_COMP_NEG1: printf("-1"); break;
        case TOKEN_COMP_D: printf("D"); break;
        case TOKEN_COMP_A: printf("A"); break;
        case TOKEN_COMP_NOT_D: printf("!D"); break;
        case TOKEN_COMP_NOT_A: printf("!A"); break;
        case TOKEN_COMP_NEG_D: printf("-D"); break;
        case TOKEN_COMP_NEG_A: printf("-A"); break;
        case TOKEN_COMP_DPLUS1: printf("D+1"); break;
        case TOKEN_COMP_APLUS1: printf("A+1"); break;
        case TOKEN_COMP_DMINUS1: printf("D-1"); break;
        case TOKEN_COMP_AMINUS1: printf("A-1"); break;
        case TOKEN_COMP_DPLUSA: printf("D+A"); break;
        case TOKEN_COMP_DMINUSA: printf("D-A"); break;
        case TOKEN_COMP_AMINUSD: printf("A-D"); break;
        case TOKEN_COMP_DANDA: printf("D&A"); break;
        case TOKEN_COMP_DORA: printf("D|A"); break;
        case TOKEN_COMP_M: printf("M"); break;
        case TOKEN_COMP_NOT_M: printf("!M"); break;
        case TOKEN_COMP_NEG_M: printf("-M"); break;
        case TOKEN_COMP_MPLUS1: printf("M+1"); break;
        case TOKEN_COMP_MMINUS1: printf("M-1"); break;
        case TOKEN_COMP_DPLUSM: printf("D+M"); break;
        case TOKEN_COMP_DMINUSM: printf("D-M"); break;
        case TOKEN_COMP_MMINUSD: printf("M-D"); break;
        case TOKEN_COMP_DANDM: printf("D&M"); break;
        case TOKEN_COMP_DORM: printf("D|M"); break;
        default: printf("UNKNOWN");
    }
    printf("\n");

    // Print JUMP field
    printf("JUMP: ");
    switch (instr->jump) {
        case TOKEN_JUMP_NULL: printf("NULL"); break;
        case TOKEN_JUMP_JGT: printf("JGT"); break;
        case TOKEN_JUMP_JEQ: printf("JEQ"); break;
        case TOKEN_JUMP_JGE: printf("JGE"); break;
        case TOKEN_JUMP_JLT: printf("JLT"); break;
        case TOKEN_JUMP_JNE: printf("JNE"); break;
        case TOKEN_JUMP_JLE: printf("JLE"); break;
        case TOKEN_JUMP_JMP: printf("JMP"); break;
        default: printf("UNKNOWN");
    }
    printf("\n");
}
