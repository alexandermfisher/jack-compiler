//
// Created by alexanderfisher on 16/03/25.
//

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ProcessStatus preprocess_line(char *line, char **processed_line, ssize_t read);
ProcessStatus lex_label(char *line, TokenTable *token_table, SymbolTable *symbol_table, const int *rom_address);
ProcessStatus lex_symbol(char **line, char **symbol);
ProcessStatus lex_a_instruction(char *line, TokenTable *token_table);
ProcessStatus lex_integer_literal(char **line, int *integer_literal);
ProcessStatus lex_c_instruction(char *line, TokenTable *token_table);
ProcessStatus lex_dest(char **line, TokenTable *token_table);
ProcessStatus lex_comp(char **line, TokenTable *token_table);
ProcessStatus lex_jump(char **line, TokenTable *token_table);
bool is_keyword(const char *symbol);


ProcessStatus lex_line(char *line, const ssize_t read, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address) {
    if (!line || read <= 0 || !token_table || !symbol_table || !rom_address) return PROCESS_ERROR;

    // Preprocess line - removes whitespace
    char *processed_line = NULL;
    ProcessStatus status = preprocess_line(line, &processed_line, read);
    if (status != PROCESS_SUCCESS) return status;

    // Check for empty line (Safe to ignore empty/comment line)
    if (!processed_line) return PROCESS_SUCCESS;

    // Lex L-instruction (Labels)
    if (*processed_line == '(') {
        status = lex_label(processed_line, token_table, symbol_table, rom_address);
        free(processed_line);
        return status;
    }

    // Lex A-instruction (@value)
    if (*processed_line == '@') {
        status = lex_a_instruction(processed_line, token_table);
        (*rom_address)++;
        free(processed_line);
        return status;
    }

    // Lex C-instruction
    status = lex_c_instruction(processed_line, token_table);
    (*rom_address)++;
    free(processed_line);
    return status;
}

ProcessStatus preprocess_line(char *line, char **processed_line, const ssize_t read) {
    if (!line || read <= 0) return PROCESS_ERROR;

    // Allocate a buffer with at most 'read' size (worst case: no spaces removed)
    char *buffer = calloc(read + 1, sizeof(char)); // +1 for null terminator
    if (!buffer) {
        perror("calloc failed");
        return PROCESS_ERROR;
    }

    // Find comment position
    const char *comment_start = strstr(line, "//");

    // Copy only non-whitespace characters up to comment start
    size_t j = 0;
    for (size_t i = 0; i < (size_t)read; i++) {
        if (comment_start && line + i >= comment_start) {
            break; // Stop copying at start of comment
        }
        if (!isspace(line[i])) {
            buffer[j++] = line[i];
        }
    }

    // If the processed line is empty, free buffer and return
    if (j == 0) {
        free(buffer);
        *processed_line = NULL;
        return PROCESS_SUCCESS;
    }

    // Ensure last character is '\n' before null terminator
    if (buffer[j - 1] != '\n') {
        buffer[j++] = '\n';
    }

    buffer[j] = '\0'; // Null-terminate
    *processed_line = buffer;
    return PROCESS_SUCCESS;
}

ProcessStatus lex_label(char *line, TokenTable *token_table, SymbolTable *symbol_table, const int *rom_address) {
    if (!line || !token_table || !symbol_table || !rom_address) return PROCESS_ERROR;

    // Tokenize '('
    if (*line != '(') return PROCESS_INVALID;
    Token *l_paren = create_token(TOKEN_LPAREN);
    if (!l_paren) return PROCESS_ERROR;
    if (!token_table_add(token_table, l_paren)) {
        free_token(l_paren);
        return PROCESS_ERROR;
    }
    line++;  // Move past '('

    // Extract symbol
    char *symbol = NULL;
    const ProcessStatus status = lex_symbol(&line, &symbol);
    if (status != PROCESS_SUCCESS) return status;

    // If it's a reserved keyword -> invalid
    if (is_keyword(symbol)) {
        fprintf(stderr, "Invalid Symbol: '%s' is a reserved hack keyword\n", symbol);
        free(symbol);
        return PROCESS_INVALID;
    }

    // Tokenize symbol
    Token *symbol_token = create_token(TOKEN_SYMBOL, symbol);
    if (!symbol_token) {
        free(symbol);
        return PROCESS_ERROR;
    }
    if (!token_table_add(token_table, symbol_token)) {
        free_token(symbol_token);
        free(symbol);
        return PROCESS_ERROR;
    }

    // Add symbol to table
    if (!symbol_table_contains(symbol_table, symbol)
        && !symbol_table_add(symbol_table, symbol, *rom_address)) {
        free(symbol);
        return PROCESS_ERROR;
    }
    free(symbol);

    // Tokenize ')'
    if (*line != ')') return PROCESS_INVALID;
    Token *r_paren = create_token(TOKEN_RPAREN);
    if (!r_paren) return PROCESS_ERROR;
    if (!token_table_add(token_table, r_paren)) {
        free_token(r_paren);
        return PROCESS_ERROR;
    }
    line++;

    // Tokenize newline
    if (*line != '\n') return PROCESS_INVALID;
    Token *newline = create_token(NEWLINE);
    if (!newline) return PROCESS_ERROR;
    if (!token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

ProcessStatus lex_symbol(char **line, char **symbol) {
    if (!line || !*line || !symbol) return PROCESS_ERROR;

    const char *start = *line;
    const char *end = strpbrk(*line, ")\n");

    // strchr(start, ')');   // Find ')'
    // if (!end) end = strchr(start, '\n');    // If no ')', find newline as fallback


    if (!start || !end || end < start) {
        return PROCESS_INVALID; // No valid symbol found
    }

    const size_t len = end - start;
    if (len == 0) {
        return PROCESS_INVALID; // Empty symbol is invalid
    }

    // Ensure symbol does not start with a digit
    if (isdigit(start[0])) {
        return PROCESS_INVALID;
    }

    // Allocate memory for the symbol
    char *label = malloc(len + 1);
    if (!label) {
        perror("malloc failed");
        return PROCESS_ERROR;
    }

    // Validate and copy character by character
    int i = 0;
    for (const char *ptr = start; ptr < end; ptr++) {
        const char c = *ptr;

        // Check if character is valid
        if (!(isalnum(c) || c == '_' || c == '.' || c == '$' || c == ':')) {
            free(label);
            return PROCESS_INVALID; // Invalid character in symbol
        }

        label[i++] = c;
    }

    label[i] = '\0';        // Null-terminate
    *symbol = label;        // Set output parameter

    *line = (char*)end;     // Move the caller's line pointer upto ')' or '\n'
    return PROCESS_SUCCESS;
}

ProcessStatus lex_a_instruction(char *line, TokenTable *token_table) {
    if (!line || !token_table) return PROCESS_ERROR;

    // Create '@' operator token
    if (*line != '@') return PROCESS_ERROR;
    Token *at_token = create_token(TOKEN_AT);
    if (!at_token) return PROCESS_ERROR;
    if (!token_table_add(token_table, at_token)) {
        free_token(at_token);
        return PROCESS_ERROR;
    }
    line++;  // Move past '@'

    // Lex @value:
    if (isdigit(*line)) {
        //Extract integer literal
        int integer_literal = 0;
        const ProcessStatus status = lex_integer_literal(&line, &integer_literal);
        if (status != PROCESS_SUCCESS) return status;

        // Tokenise integer literal
        Token *int_token = create_token(TOKEN_INTEGER, integer_literal);
        if (!int_token) return PROCESS_ERROR;
        if (!token_table_add(token_table, int_token)) {
            free_token(int_token);
            return PROCESS_ERROR;
        }
    } else {
        // Extract symbol
        char *symbol = NULL;
        const ProcessStatus status = lex_symbol(&line, &symbol);
        if (status != PROCESS_SUCCESS) return status;

        // Tokenize symbol
        Token *symbol_token = create_token(TOKEN_SYMBOL, symbol);
        if (!symbol_token) {
            free(symbol);
            return PROCESS_ERROR;
        }
        free(symbol);
        if (!token_table_add(token_table, symbol_token)) {
            free_token(symbol_token);
            return PROCESS_ERROR;
        }
    }

    // Tokenize newline
    if (*line != '\n') return PROCESS_INVALID;
    Token *newline = create_token(NEWLINE);
    if (!newline) return PROCESS_ERROR;
    if (!token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

ProcessStatus lex_integer_literal(char **line, int *integer_literal) {
    if (!line || !*line || !integer_literal) return PROCESS_ERROR;

    if (!isdigit(**line)) return PROCESS_INVALID;

    char number[6] = {0};  // Max "32767" + null terminator
    int i = 0;
    while (isdigit(**line) && i < 5) {
        number[i++] = **line;
        (*line)++;
    }
    number[i] = '\0';

    // Convert to int
    char *end_ptr;
    const long num = strtol(number, &end_ptr, 10);
    if (*end_ptr != '\0' || num < 0 || num >= 32768) return PROCESS_INVALID;
    *integer_literal = (int) num;

    return PROCESS_SUCCESS;
}

ProcessStatus lex_c_instruction(char *line, TokenTable *token_table) {
    if (!line || !token_table) return PROCESS_ERROR;

    // Process dest
    ProcessStatus status = lex_dest(&line, token_table);
    if (status != PROCESS_SUCCESS) return status;

    // Process comp
    status = lex_comp(&line, token_table);
    if (status != PROCESS_SUCCESS) return status;

    // Process jump
    status = lex_jump(&line, token_table);
    if (status != PROCESS_SUCCESS) return status;

    // Tokenize newline
    if (*line != '\n') return PROCESS_INVALID;
    Token *newline = create_token(NEWLINE);
    if (!newline) return PROCESS_ERROR;
    if (!token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

ProcessStatus lex_dest(char **line, TokenTable *token_table) {
    if (!line || !*line || !token_table) return PROCESS_ERROR;

    char *eq_pos = strchr(*line, '=');
    if (!eq_pos) {
        Token *null_token = create_token(TOKEN_DEST_NULL);
        if (!null_token) return PROCESS_ERROR;
        if (!token_table_add(token_table, null_token)) {
            free(null_token);
            return PROCESS_ERROR;
        }
        return PROCESS_SUCCESS;
    }

    // Extract potential destination (everything before `=`)
    const size_t dest_len = eq_pos - *line;
    if (dest_len == 0 || dest_len > 3) return PROCESS_INVALID; // Empty or too long

    char dest[4] = {0}; // Max valid dest length is 3 ("AMD"), +1 for null terminator
    strncpy(dest, *line, dest_len);
    dest[dest_len] = '\0';

    // List of valid destinations
    const struct {
        const char *name;
        TokenType type;
    } valid_dests[] = {
        {"M",   TOKEN_DEST_M},
        {"D",   TOKEN_DEST_D},
        {"MD",  TOKEN_DEST_MD},
        {"A",   TOKEN_DEST_A},
        {"AM",  TOKEN_DEST_AM},
        {"AD",  TOKEN_DEST_AD},
        {"AMD", TOKEN_DEST_AMD}
    };

    // Validate and create token
    for (size_t i = 0; i < sizeof(valid_dests) / sizeof(valid_dests[0]); i++) {
        if (strcmp(dest, valid_dests[i].name) == 0) {
            Token *token = create_token(valid_dests[i].type);
            if (!token) return PROCESS_ERROR;
            if (!token_table_add(token_table, token)) {
                free(token);
                return PROCESS_ERROR;
            }
            *line = eq_pos + 1; // Move to '='
            return PROCESS_SUCCESS;
        }
    }

    return PROCESS_INVALID; // Invalid destination
}

ProcessStatus lex_comp(char **line, TokenTable *token_table) {
    if (!line || !*line || !token_table) return PROCESS_ERROR;

    char *end = strpbrk(*line, ";\n");  // Find `;` or newline
    const size_t comp_len = end ? (size_t)(end - *line) : strlen(*line);

    if (comp_len == 0) return PROCESS_INVALID;  // Empty comp is invalid

    char comp[5] = {0};  // Max length of comp mnemonics is 4 ("D|M"), +1 for null
    if (comp_len > 4) return PROCESS_INVALID;  // Too long for a valid comp

    strncpy(comp, *line, comp_len);
    comp[comp_len] = '\0';

    // List of valid comp mnemonics
    const struct {
        const char *name;
        TokenType type;
    } valid_comps[] = {
        {"0",    TOKEN_COMP_0},   {"1",    TOKEN_COMP_1},
        {"-1",   TOKEN_COMP_NEG1},{"D",    TOKEN_COMP_D},
        {"A",    TOKEN_COMP_A},   {"M",    TOKEN_COMP_M},
        {"!D",   TOKEN_COMP_NOT_D},{"!A",   TOKEN_COMP_NOT_A},
        {"!M",   TOKEN_COMP_NOT_M},{"-D",   TOKEN_COMP_NEG_D},
        {"-A",   TOKEN_COMP_NEG_A},{"-M",   TOKEN_COMP_NEG_M},
        {"D+1",  TOKEN_COMP_DPLUS1},{"A+1", TOKEN_COMP_APLUS1},
        {"M+1", TOKEN_COMP_MPLUS1},{"D-1",  TOKEN_COMP_DMINUS1},
        {"A-1",TOKEN_COMP_AMINUS1},{"M-1", TOKEN_COMP_MMINUS1},
        {"D+A",  TOKEN_COMP_DPLUSA},{"D+M", TOKEN_COMP_DPLUSM},
        {"D-A", TOKEN_COMP_DMINUSA},{"D-M",  TOKEN_COMP_DMINUSM},
        {"A-D",TOKEN_COMP_AMINUSD},{"M-D", TOKEN_COMP_MMINUSD},
        {"D&A",  TOKEN_COMP_DANDA},{"D&M", TOKEN_COMP_DANDM},
        {"D|A", TOKEN_COMP_DORA},{"D|M",  TOKEN_COMP_DORM}
    };

    // Validate and create token
    for (size_t i = 0; i < sizeof(valid_comps) / sizeof(valid_comps[0]); i++) {
        if (strcmp(comp, valid_comps[i].name) == 0) {
            Token *token = create_token(valid_comps[i].type);
            if (!token) return PROCESS_ERROR;
            if (!token_table_add(token_table, token)) {
                free(token);
                return PROCESS_ERROR;
            }
            *line = end ? end : *line + comp_len;  // Move past comp
            return PROCESS_SUCCESS;
        }
    }

    return PROCESS_INVALID; // Invalid comp
}

ProcessStatus lex_jump(char **line, TokenTable *token_table) {
    if (!line || !*line || !token_table) return PROCESS_ERROR;

    // If no jump set TOKEN_JUMP_NULL
    if (**line != ';') {
        Token *null_token = create_token(TOKEN_JUMP_NULL);
        if (!null_token) return PROCESS_ERROR;
        if (!token_table_add(token_table, null_token)) {
            free(null_token);
            return PROCESS_ERROR;
        }
        return PROCESS_SUCCESS;
    }
    (*line)++; // Move past ';'

    // Valid jump length is 3 ("JMP"), +1 for null terminator
    if (strlen(*line) - 1 != 3) return PROCESS_INVALID;
    char jump[4] = {0};
    strncpy(jump, *line, 3);
    jump[3] = '\0';

    // List of valid destinations
    const struct {
        const char *name;
        TokenType type;
    } valid_jumps[] = {
        {"JGT",   TOKEN_JUMP_JGT},
        {"JEQ",   TOKEN_JUMP_JEQ},
        {"JGE",  TOKEN_JUMP_JGE},
        {"JLT",   TOKEN_JUMP_JLT},
        {"JNE",  TOKEN_JUMP_JNE},
        {"JLE",  TOKEN_JUMP_JLE},
        {"JMP", TOKEN_JUMP_JMP}
    };

    // Validate and create token
    for (size_t i = 0; i < sizeof(valid_jumps) / sizeof(valid_jumps[0]); i++) {
        if (strcmp(jump, valid_jumps[i].name) == 0) {
            Token *token = create_token(valid_jumps[i].type);
            if (!token) return PROCESS_ERROR;
            if (!token_table_add(token_table, token)) {
                free(token);
                return PROCESS_ERROR;
            }
            *line = *line + 3; // Move past ';'
            return PROCESS_SUCCESS;
        }
    }

    return PROCESS_INVALID;
}

bool is_keyword(const char *symbol) {
    static const char *keywords[] = {
        "A", "M", "D", "AMD", "AD", "MD", "AM", "JMP", "JEQ", "JGT", "JGE",
        "JLT", "JLE", "JNE", "NULL", "THIS", "THAT", "R0", "R1", "R2", "R3",
        "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15",
        "SCREEN", "KBD", "SP", "LCL", "ARG", "TEMP"
    };

    const size_t num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    for (size_t i = 0; i < num_keywords; i++) {
        if (strcmp(symbol, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}
