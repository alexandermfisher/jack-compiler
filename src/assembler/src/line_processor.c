//
// Created by Alexander Fisher on 09/03/2025.
//
#include "line_processor.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

ProcessStatus process_line(const char *line, TokenTable *table) {
    if (!line || !table) return PROCESS_ERROR;

    line = preprocess_line(line);

    // Safe to ignore (empty/comment line)
    if (!line) return PROCESS_EMPTY;

    // Check for L-instruction (Labels)
    if (*line == '(') return process_label(line, table);


    // Check for A-instruction (@value)
    if (*line == '@') return process_a_instruction(line, table);

    // TODO: Handle C-instructions...

    return PROCESS_SUCCESS;
}

// Function to trim leading whitespace and handle empty/comment lines
const char *preprocess_line(const char *line) {
    // Skip leading whitespace
    while (*line == ' ' || *line == '\t') line++;
    // Skip empty lines and comments
    if (*line == '\0' || strncmp(line, "//", 2) == 0) return NULL;

    return line;
}

ProcessStatus process_label(const char *line, TokenTable *table) {
    if (!line || *line != '(') return PROCESS_ERROR;

    // Tokenise '('
    Token *l_paren = create_token(SEPARATOR, SEP_LPAREN);
    if (!l_paren || !token_table_add(table, l_paren)) {
        free_token(l_paren);
        return PROCESS_ERROR;
    }



}









ProcessStatus process_a_instruction(const char *line, TokenTable *table) {
    if (!line || *line != '@') return PROCESS_ERROR;

    // Create '@' operator token
    Token *at_token = create_token(OPERATOR, OP_AT);
    if (!at_token || !token_table_add(table, at_token)) {
        free_token(at_token);
        return PROCESS_ERROR;
    }

    line++;  // Move past '@'

    // Integer literal (e.g., @32767)
    if (isdigit(*line)) {
        char number[6] = {0};  // Max "32767" + null terminator
        int i = 0;
        while (isdigit(*line) && i < 5) number[i++] = *line++;
        number[i] = '\0';

        // Ensure valid termination: only allow EOL, space, newline, or valid comment start
        if (*line == '\0' || *line == ' ' || *line == '\n' || (*line == '/' && *(line + 1) == '/')) {
            const int num = strtol(number, NULL, 10);
            Token *int_token = create_token(INTEGER_LITERAL, num);
            if (!int_token || !token_table_add(table, int_token)) {
                free_token(int_token);
                return PROCESS_ERROR;
            }
            return PROCESS_SUCCESS;
        }
        return PROCESS_INVALID;  // Invalid case like @5ABC or @5/
    }

    // Symbol (e.g., @LOOP, @var)
    if (isalpha(*line) || *line == '_' || *line == '.' || *line == '$' || *line == ':') {
        char symbol[64] = {0};
        int i = 0;
        while ((isalnum(*line) || *line == '_' || *line == '.' || *line == '$' || *line == ':') && i < 63) {
            symbol[i++] = *line++;
        }
        symbol[i] = '\0';  // Null-terminate

        // Ensure valid termination (EOL, space, newline, or comment start)
        if (*line == '\0' || *line == ' ' || *line == '\n' || (*line == '/' && *(line + 1) == '/')) {
            Token *sym_token = create_token(SYMBOL, symbol);
            if (!sym_token || !token_table_add(table, sym_token)) {
                free_token(sym_token);
                return PROCESS_ERROR;
            }
            return PROCESS_SUCCESS;
        }
        return PROCESS_ERROR;
    }
    return PROCESS_SUCCESS;
}







