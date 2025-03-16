//
// Created by alexanderfisher on 16/03/25.
//

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ProcessStatus preprocess_line(const char *line, char **processed_line, ssize_t read);
ProcessStatus lex_label(const char **line, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address);
ProcessStatus lex_symbol(const char* line, char **symbol);
bool is_keyword(const char *symbol);


ProcessStatus lex_line(const char *line, const ssize_t read, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address) {
    if (!line || read <= 0 || !token_table || !symbol_table || !rom_address) return PROCESS_ERROR;

    // Preprocess line - removes whitespace
    char *processed_line = NULL;
    ProcessStatus status = preprocess_line(line, &processed_line, read);
    if (status != PROCESS_SUCCESS) return status;

    // Check for empty line (Safe to ignore empty/comment line)
    if (processed_line[0] == '\0') {
        free(processed_line);
        return PROCESS_SUCCESS;
    }

    // Check for L-instruction (Labels)
    if (*processed_line == '(') {
        status = lex_label(&processed_line, token_table, symbol_table, rom_address);
        free(processed_line);
        return status;
    }

    // Check for A-instruction (@value)
    if (*line == '@') {

        free(processed_line);
        return status;
    }

    *rom_address++;
    return PROCESS_SUCCESS;
}


// Function to preprocess line: remove whitespace, strip comments, ensure newline
ProcessStatus preprocess_line(const char *line, char **processed_line, const ssize_t read) {
    if (!line || read <= 0) return PROCESS_ERROR;

    // Syntax error: input must end with a newline
    if (line[read - 1] != '\n') return PROCESS_INVALID;

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
        if (!isspace((unsigned char)line[i])) {
            buffer[j++] = line[i];
        }
    }

    buffer[j] = '\0'; // Null-terminate
    *processed_line = buffer;
    return PROCESS_SUCCESS;
}


// Lexing L-INSTRUCTIONS
ProcessStatus lex_label(const char **line, TokenTable *token_table, SymbolTable *symbol_table, const int *rom_address) {
    if (!*line || !token_table || !symbol_table || !rom_address) return PROCESS_ERROR;

    // Tokenize '('
    if (**line != '(') return PROCESS_INVALID;
    Token *l_paren = create_token(TOKEN_LPAREN);
    if (!l_paren) return PROCESS_ERROR;
    if (!token_table_add(token_table, l_paren)) {
        free_token(l_paren);
        return PROCESS_ERROR;
    }
    (*line)++;  // Move past '('

    // Extract symbol
    char *symbol = NULL;
    const ProcessStatus status = lex_symbol(*line, &symbol);
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
    if (**line != ')') return PROCESS_INVALID;
    Token *r_paren = create_token(TOKEN_RPAREN);
    if (!r_paren) return PROCESS_ERROR;
    if (!token_table_add(token_table, r_paren)) {
        free_token(r_paren);
        return PROCESS_ERROR;
    }
    (*line)++;

    // Tokenize newline
    if (**line != '\n') return PROCESS_INVALID;
    Token *newline = create_token(NEWLINE);
    if (!newline) return PROCESS_ERROR;
    if (!token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

ProcessStatus lex_symbol(char **line, char **symbol) {
    const char *start = *line;
    const char *end = strchr(*line, ')');

    if (!start || !end || end < start) {
        return PROCESS_INVALID; // No valid symbol found
    }

    const size_t len = end - start - 1;
    if (len == 0) {
        return PROCESS_INVALID; // Empty symbol is invalid
    }

    // Ensure symbol does not start with a digit
    if (isdigit(start[1])) {
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
    for (const char *ptr = start + 1; ptr < end; ptr++) {
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

    *line = (char*)end;     // Move the caller's line pointer upto ')'
    return PROCESS_SUCCESS;
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


// Lexing A-INSTRUCTIONS
ProcessStatus process_a_instruction(const char **line, TokenTable *token_table) {
    if (!*line || !token_table) return PROCESS_ERROR;

    if (**line != '@') return PROCESS_ERROR;

    // Create '@' operator token
    Token *at_token = create_token(TOKEN_AT);
    if (!at_token) return PROCESS_ERROR;
    if (!token_table_add(token_table, at_token)) {
        free_token(at_token);
        return PROCESS_ERROR;
    }
    (*line)++;  // Move past '@'



    // TODO process integer literal
    // Process integer literal:
    if (isdigit(**line)) {
        int integer_literal = 0;
        const ProcessStatus status = process_integer_literal(line, &integer_literal);
        if (status != PROCESS_SUCCESS) return status;

        // Tokenise integer literal
        Token *int_token = create_token(TOKEN_INTEGER, integer_literal);
        if (!int_token || !token_table_add(token_table, int_token)) {
            free_token(int_token);
            return PROCESS_ERROR;
        }
    } else {
        // Process symbol
        char *symbol = NULL;
        const ProcessStatus status = lex_symbol(*line, &symbol);
        if (status != PROCESS_SUCCESS) return status;

        // Tokenize symbol
        Token *symbol_token = create_token(TOKEN_SYMBOL, symbol);
        if (!symbol || !token_table_add(token_table, symbol_token)) {
            free_token(symbol_token);
            return PROCESS_ERROR;
        }
    }


    // Tokenize newline
    if (**line != '\n') return PROCESS_INVALID;
    Token *newline = create_token(NEWLINE);
    if (!newline) return PROCESS_ERROR;
    if (!token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

