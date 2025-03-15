#include <stdlib.h>
#include "parser.h"
#include "line_processor.h"

#define MAX_TOKENS_PER_INSTRUCTION 10

bool parse_l_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]);
bool parse_a_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]);
bool parse_c_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]);
bool parse_dest(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token);
bool parse_comp(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token);
bool parse_unary_comp(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token, int comp_count);
bool parse_binary_comp(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token);

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


    // static int instruction_count = 1;
    // printf("Instruction %d:\n", instruction_count);
    // for (int i = 0; i < token_count; i++) {
    //     char *token_str = token_to_str(tokens[i]);
    //     printf("\t\t%s\n", token_str);
    //     free(token_str);
    // }
    // printf("\n");
    // instruction_count++;

    // Parse L-instruction:
    if (parse_l_instruction(parser, tokens)) {
        print_l_instruction(parser->instruction);
        return true;
    }

    // Parse A-instruction:
    if (parse_a_instruction(parser, tokens)) {
        print_a_instruction(parser->instruction);
        return true;
    }

    if (parse_c_instruction(parser, tokens)) {
        print_c_instruction(parser->instruction);
        return true;
    }


    return false;
}

bool parse_l_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]) {

    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;
    int current_token = 0;

    if (tokens[current_token]->type != SEPARATOR ||
        tokens[current_token++]->value.separator != SEP_LPAREN) {
        return false;
    }

    instruction->type = L_INSTRUCTION;

    if (tokens[current_token]->type != SYMBOL) return false;
    instruction->symbol = tokens[current_token++]->value.symbol;

    if (tokens[current_token]->type != SEPARATOR ||
        tokens[current_token++]->value.separator != SEP_RPAREN) {
        return false;
    }

    if (tokens[current_token]->type != NEWLINE) return false;

    return true;
}

bool parse_a_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]) {
    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;
    int current_token = 0;

    if (tokens[current_token]->type != OPERATOR ||
        tokens[current_token++]->value.operator != OP_AT) return false;

    if (tokens[current_token]->type == INTEGER_LITERAL) {
        instruction->type = A_INSTRUCTION_VALUE;
        instruction->value = tokens[current_token++]->value.integer;
    } else if (tokens[current_token]->type == SYMBOL) {
        instruction->type = A_INSTRUCTION_SYMBOL;
        instruction->symbol = tokens[current_token++]->value.symbol;
    } else return false;

    if (tokens[current_token]->type != NEWLINE) return false;

    return true;
}

bool parse_c_instruction(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION]) {
    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;
    int current_token = 0;

    // Default values
    instruction->type = C_INSTRUCTION;
    instruction->dest = NULL_DEST;
    instruction->comp = COMP_0;
    instruction->jump = NO_JUMP;

    // Parse for optional DEST field
    if (!parse_dest(parser, tokens, &current_token)) return false;

    // Parse mandatory COMP field
    if (!parse_comp(parser, tokens, &current_token)) return false;

    // Parse for optional JUMP field (`;JUMP`)**
    if (tokens[current_token] && tokens[current_token]->type == SEPARATOR &&
        tokens[current_token]->value.operator == SEP_SEMICOLON) {
        current_token++;

        if (tokens[current_token] && tokens[current_token]->type == KEYWORD) {
            if (tokens[current_token]->value.keyword == KW_JGT) instruction->jump = JGT;
            else if (tokens[current_token]->value.keyword == KW_JEQ) instruction->jump = JEQ;
            else if (tokens[current_token]->value.keyword == KW_JGE) instruction->jump = JGE;
            else if (tokens[current_token]->value.keyword == KW_JLT) instruction->jump = JLT;
            else if (tokens[current_token]->value.keyword == KW_JNE) instruction->jump = JNE;
            else if (tokens[current_token]->value.keyword == KW_JLE) instruction->jump = JLE;
            else if (tokens[current_token]->value.keyword == KW_JMP) instruction->jump = JMP;
            else return false; // Invalid jump
        } else {
            return false; // JUMP must exist after ';'
        }
        current_token++;
    }

    // Ensure instruction ends with NEWLINE
    if (!tokens[current_token] || tokens[current_token]->type != NEWLINE) {
        return false;
    }

    return true;
}

bool parse_dest(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token) {
    if (!parser || !tokens) return false;

    Instruction *instruction = parser->instruction;

    // Check if '=' exists (indicating DEST is present)
    int assign_index = -1;
    for (int i = 0; i < MAX_TOKENS_PER_INSTRUCTION; i++) {
        if (tokens[i]->type == NEWLINE) break;
        if (tokens[i]->type == OPERATOR && tokens[i]->value.operator == OP_ASSIGN) {
            assign_index = i;
            break;
        }
    }
    if (assign_index == -1) return true;  // No destination field

    // Track encountered registers
    bool has_M = false, has_D = false;
    Dest parsed_dest = NULL_DEST;

    // Process tokens before '='
    for (int i = 0; i < assign_index; i++) {
        if (tokens[i]->type != KEYWORD) return false; // Invalid component

        if (tokens[i]->value.keyword == KW_A) {
            if (has_M || has_D) return false;  // A must come first
            parsed_dest |= DEST_A;
        }
        else if (tokens[i]->value.keyword == KW_M) {
            if (has_D) return false;  // M must come before D
            has_M = true;
            parsed_dest |= DEST_M;
        }
        else if (tokens[i]->value.keyword == KW_D) {
            has_D = true;
            parsed_dest |= DEST_D;
        }
        else {
            return false;  // Invalid keyword
        }
    }

    // Validate parsed destination against valid sets
    if (parsed_dest != DEST_A && parsed_dest != DEST_M && parsed_dest != DEST_D &&
        parsed_dest != DEST_AM && parsed_dest != DEST_MD && parsed_dest != DEST_AMD &&
        parsed_dest != DEST_AD) {
        return false;  // Invalid combination
    }

    instruction->dest = parsed_dest;
    *current_token = assign_index + 1;  // Move past '='
    return true;
}

bool parse_comp(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token) {
    if (!parser || !tokens || !tokens[*current_token]) return false;

    // Count tokens in comp
    int comp_count = 0;
    for (int i = *current_token; i < MAX_TOKENS_PER_INSTRUCTION; i++) {
        if ((tokens[i]->type == SEPARATOR && tokens[i]->value.separator == SEP_SEMICOLON)
            || tokens[i]->type == NEWLINE) break;
        comp_count++;
    }

    if (comp_count <= 2) return parse_unary_comp(parser, tokens, current_token, comp_count);
    if (comp_count == 3) return parse_binary_comp(parser, tokens, current_token);

    return false;
}

bool parse_unary_comp(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token, const int comp_count) {
    if (!parser || !tokens || !tokens[*current_token]) return false;

    Comp comp;

    // Must be (A, D, M, or 1, 0)
    if (comp_count == 1) {
        if (tokens[*current_token]->type == INTEGER_LITERAL) {
            // Handle numeric literals (0, 1)
            const int comp_value = tokens[*current_token]->value.integer;
            if (comp_value == 0) comp = COMP_0;
            else if (comp_value == 1) comp = COMP_1;
            else return false; // Invalid constant
        } else if (tokens[*current_token]->type == KEYWORD) {
            // Handle register keywords
            if (tokens[*current_token]->value.keyword == KW_D) comp = COMP_D;
            else if (tokens[*current_token]->value.keyword == KW_A) comp = COMP_A;
            else if (tokens[*current_token]->value.keyword == KW_M) comp = COMP_M;
            else return false; // Invalid comp
        } else {
            return false; // Invalid component
        }

        parser->instruction->comp = comp;
        (*current_token)++;
        return true;
    }

    // Must be (-1, -A, -D, -M, or !A, !D, !M)

    // Unary operation must be '-' or '!'
    if (tokens[*current_token]->type != OPERATOR || (tokens[*current_token]->value.operator != OP_SUB &&
        tokens[*current_token]->value.operator != OP_NOT)) return false;
    const Operator unary_op = tokens[*current_token]->value.operator;
    (*current_token)++;

    // Handle '-1'
    if (unary_op == OP_SUB && tokens[*current_token]->type == INTEGER_LITERAL) {
        const int comp_value = tokens[*current_token]->value.integer;
        if (comp_value == 1) {
            comp = COMP_MINUS1;
            parser->instruction->comp = comp;
            (*current_token)++;
            return true;
        }
        return false;
    }

    // Handle register keywords (-A, -D, -M, or !A, !D, !M)
    if (tokens[*current_token]->type != KEYWORD) return false;
    if (tokens[*current_token]->value.keyword == KW_D) comp = COMP_D;
    else if (tokens[*current_token]->value.keyword == KW_A) comp = COMP_A;
    else if (tokens[*current_token]->value.keyword == KW_M) comp = COMP_M;
    else return false;

    if (unary_op == OP_NOT) {
        if (comp == COMP_D) comp = COMP_NOT_D;
        else if (comp == COMP_A) comp = COMP_NOT_A;
        else comp = COMP_NOT_M;
    } else {
        if (comp == COMP_D) comp = COMP_MINUS_D;
        else if (comp == COMP_A) comp = COMP_MINUS_A;
        else comp = COMP_MINUS_M;
    }

    parser->instruction->comp = comp;
    (*current_token)++;
    return true;
}

bool parse_binary_comp(Parser *parser, Token *tokens[MAX_TOKENS_PER_INSTRUCTION], int *current_token) {
    if (!parser || !tokens || !tokens[*current_token]) return false;

    // First operand must be a register (A, D, or M)
    if (tokens[*current_token]->type != KEYWORD) return false;
    Comp lhs_comp;
    if (tokens[*current_token]->value.keyword == KW_D) lhs_comp = COMP_D;
    else if (tokens[*current_token]->value.keyword == KW_A) lhs_comp = COMP_A;
    else if (tokens[*current_token]->value.keyword == KW_M) lhs_comp = COMP_M;
    else return false;
    (*current_token)++;  // Move to the next token


    // Next token must be a valid binary operator (+, -, &, |)
    if (tokens[*current_token]->type != OPERATOR) return false;
    const Operator bin_op = tokens[*current_token]->value.operator;
    (*current_token)++;  // Move to the next token


    // Handle D+1, A+1, D-1, A-1, M+1, M-1
    if (tokens[*current_token]->type == INTEGER_LITERAL) {
        if (tokens[*current_token]->value.integer != 1) return false;
        Comp comp;
        if (bin_op == OP_ADD) {
            if (lhs_comp == COMP_D) comp = COMP_D_PLUS_1;
            else if (lhs_comp == COMP_A) comp = COMP_A_PLUS_1;
            else comp = COMP_M_PLUS_1;
        } else if (bin_op == OP_SUB) {
            if (lhs_comp == COMP_D) comp = COMP_D_MINUS_1;
            else if (lhs_comp == COMP_A) comp = COMP_A_MINUS_1;
            else comp = COMP_M_MINUS_1;
        } else return false;

        // Store the computed comp value
        parser->instruction->comp = comp;
        (*current_token)++;  // Move past the second operand
        return true;
    }

    // Second operand must also be a register (A, D, or M)
    if (tokens[*current_token]->type != KEYWORD) return false;
    Comp rhs_comp;
    if (tokens[*current_token]->value.keyword == KW_D) rhs_comp = COMP_D;
    else if (tokens[*current_token]->value.keyword == KW_A) rhs_comp = COMP_A;
    else if (tokens[*current_token]->value.keyword == KW_M) rhs_comp = COMP_M;
    else return false;

    // Determine the correct comp code based on the binary operation
    Comp comp;
    if (bin_op == OP_ADD) {
        if (lhs_comp == COMP_D && rhs_comp == COMP_A) comp = COMP_D_PLUS_A;
        else if (lhs_comp == COMP_M && rhs_comp == COMP_D) comp = COMP_D_PLUS_M;
        else return false;
    } else if (bin_op == OP_SUB) {
        if (lhs_comp == COMP_D && rhs_comp == COMP_A) comp = COMP_D_MINUS_A;
        else if (lhs_comp == COMP_A && rhs_comp == COMP_D) comp = COMP_A_MINUS_D;
        else if (lhs_comp == COMP_D && rhs_comp == COMP_M) comp = COMP_D_MINUS_M;
        else if (lhs_comp == COMP_M && rhs_comp == COMP_D) comp = COMP_M_MINUS_D;
        else return false;
    } else if (bin_op == OP_AND) {
        if (lhs_comp == COMP_D && rhs_comp == COMP_A) comp = COMP_D_AND_A;
        else if (lhs_comp == COMP_D && rhs_comp == COMP_M) comp = COMP_D_AND_M;
        else return false;
    } else if (bin_op == OP_OR) {
        if (lhs_comp == COMP_D && rhs_comp == COMP_A) comp = COMP_D_OR_A;
        else if (lhs_comp == COMP_D && rhs_comp == COMP_M) comp = COMP_D_OR_M;
        else return false;
    } else {
        return false; // Invalid operator
    }

    // Store the computed comp value
    parser->instruction->comp = comp;
    (*current_token)++;  // Move past the second operand
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
    if (instr->dest == NULL_DEST) {
        printf("NULL");
    } else {
        if (instr->dest & DEST_A) printf("A");
        if (instr->dest & DEST_M) printf("M");
        if (instr->dest & DEST_D) printf("D");
    }
    printf("\n");

    // Print COMP field
    printf("COMP: ");
    switch (instr->comp) {
        case COMP_0: printf("0"); break;
        case COMP_1: printf("1"); break;
        case COMP_MINUS1: printf("-1"); break;
        case COMP_D: printf("D"); break;
        case COMP_A: printf("A"); break;
        case COMP_NOT_D: printf("!D"); break;
        case COMP_NOT_A: printf("!A"); break;
        case COMP_MINUS_D: printf("-D"); break;
        case COMP_MINUS_A: printf("-A"); break;
        case COMP_D_PLUS_1: printf("D+1"); break;
        case COMP_A_PLUS_1: printf("A+1"); break;
        case COMP_D_MINUS_1: printf("D-1"); break;
        case COMP_A_MINUS_1: printf("A-1"); break;
        case COMP_D_PLUS_A: printf("D+A"); break;
        case COMP_D_MINUS_A: printf("D-A"); break;
        case COMP_A_MINUS_D: printf("A-D"); break;
        case COMP_D_AND_A: printf("D&A"); break;
        case COMP_D_OR_A: printf("D|A"); break;
        case COMP_M: printf("M"); break;
        case COMP_NOT_M: printf("!M"); break;
        case COMP_MINUS_M: printf("-M"); break;
        case COMP_M_PLUS_1: printf("M+1"); break;
        case COMP_M_MINUS_1: printf("M-1"); break;
        case COMP_D_PLUS_M: printf("D+M"); break;
        case COMP_D_MINUS_M: printf("D-M"); break;
        case COMP_M_MINUS_D: printf("M-D"); break;
        case COMP_D_AND_M: printf("D&M"); break;
        case COMP_D_OR_M: printf("D|M"); break;
        default: printf("UNKNOWN");
    }
    printf("\n");

    // Print JUMP field
    printf("JUMP: ");
    switch (instr->jump) {
        case NO_JUMP: printf("NULL"); break;
        case JGT: printf("JGT"); break;
        case JEQ: printf("JEQ"); break;
        case JGE: printf("JGE"); break;
        case JLT: printf("JLT"); break;
        case JNE: printf("JNE"); break;
        case JLE: printf("JLE"); break;
        case JMP: printf("JMP"); break;
        default: printf("UNKNOWN");
    }
    printf("\n");
}

