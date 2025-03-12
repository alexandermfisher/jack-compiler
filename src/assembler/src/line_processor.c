//
// Created by Alexander Fisher on 09/03/2025.
//
#include "line_processor.h"
#include "config.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


ProcessStatus process_line(const char *line, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address) {
    if (!line || !token_table) return PROCESS_ERROR;

    // Trim leading whitespace (Safe to ignore empty/comment line)
    line = preprocess_line(line);
    if (!line) return PROCESS_SUCCESS;

    // Check for L-instruction (Labels)
    if (*line == '(') return process_label(&line, token_table, symbol_table, rom_address);

    // Check for A-instruction (@value)
    if (*line == '@') return process_a_instruction(&line, token_table);

    // TODO: Handle C-instructions...

    return PROCESS_SUCCESS;
}

const char *preprocess_line(const char *line) {
    // Skip leading whitespace
    while (*line == ' ' || *line == '\t') line++;
    // Skip empty lines and comments
    if (*line == '\0' || strncmp(line, "//", 2) == 0) return NULL;

    return line;
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

bool is_line_end_or_comment(const char *line) {
    while (*line) {
        if (isspace(*line)) {
            line++;  // Skip whitespace
        } else if (*line == '/' && *(line + 1) == '/') {
            return true;  // Found a comment, valid
        } else {
            return false; // Found an invalid character
        }
    }
    return true;  // If only whitespace remains, it's valid
}

void consume_whitespace(const char **line) {
    while (isspace(**line))
        (*line)++;
}

bool is_valid_c_instruction_char(const char c, const char valid_chars[], const int n_valid_chars) {
    for (int i = 0; i < n_valid_chars; i++) {
        if (c == valid_chars[i]) return true;
    }
    return false;
}


ProcessStatus process_label(const char **line, TokenTable *token_table, SymbolTable *symbol_table, int *rom_address) {
    if (!*line || !token_table || !symbol_table || !rom_address) return PROCESS_ERROR;

    if (**line != '(') return PROCESS_INVALID;

    // Tokenize '('
    Token *l_paren = create_token(SEPARATOR, SEP_LPAREN);
    if (!l_paren || !token_table_add(token_table, l_paren)) {
        free_token(l_paren);
        return PROCESS_ERROR;
    }

    (*line)++;  // Move past '('

    // Read label into buffer
    char buffer[MAX_LABEL_LEN + 1] = {0};
    const ProcessStatus status = process_symbol(line, buffer);
    if (status != PROCESS_SUCCESS) return status;

    // Check if it's a reserved keyword
    if (is_keyword(buffer)) {
        fprintf(stderr, "Invalid Symbol: '%s' is a reserved hack keyword\n", buffer);
        return PROCESS_INVALID;
    }

    // Tokenize symbol
    Token *symbol = create_token(SYMBOL, buffer);
    if (!symbol || !token_table_add(token_table, symbol)) {
        free_token(symbol);
        return PROCESS_ERROR;
    }

    // Add symbol to table
    if (!symbol_table_contains(symbol_table, buffer)) {
        if (!symbol_table_add(symbol_table, buffer, *rom_address)) {
            return PROCESS_ERROR;
        }
        (*rom_address)++;
    }

    // Expect and tokenize ')'
    if (**line != ')') return PROCESS_INVALID;

    Token *r_paren = create_token(SEPARATOR, SEP_RPAREN);
    if (!r_paren || !token_table_add(token_table, r_paren)) {
        free_token(r_paren);
        return PROCESS_ERROR;
    }
    (*line)++;

    // Ensure only whitespace or comments remain
    if (!is_line_end_or_comment(*line)) {
        return PROCESS_INVALID;
    }

    // Tokenize newline
    Token *newline = create_token(NEWLINE, NULL);
    if (!newline || !token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

ProcessStatus process_symbol(const char **line, char *buffer) {
    if (!*line || !buffer) return PROCESS_ERROR;

    consume_whitespace(line);

    // Symbols cannot begin with a digit
    if (isdigit(**line)) return PROCESS_INVALID;

    int i = 0;
    while (isspace(**line) || isalnum(**line) || **line == '_' || **line == '.' || **line == '$' || **line == ':') {
        if (isspace(**line)) {
            consume_whitespace(line);
            continue;
        }
        if (i == MAX_LABEL_LEN) {
            fprintf(stderr, "Buffer Overflow: Label exceeds MAX_LABEL_LEN: %d\n", MAX_LABEL_LEN);
            return PROCESS_ERROR;
        }
        buffer[i++] = **line;
        (*line)++;
    }
    buffer[i] = '\0';

    // If no valid symbol was found, return invalid
    return (i > 0) ? PROCESS_SUCCESS : PROCESS_INVALID;
}

ProcessStatus process_integer_literal(const char **line, int *integer_literal) {
    if (!line || !*line || !integer_literal) return PROCESS_ERROR;

    if (!isdigit(**line)) return PROCESS_INVALID;

    char number[6] = {0};  // Max "32767" + null terminator
    int i = 0;
    while ((isspace(**line) ||isdigit(**line)) && i < 5) {
        if (isspace(**line)) {
            consume_whitespace(line);
            continue;
        }
        number[i++] = **line;
        (*line)++;
    }
    number[i] = '\0';

    // Ensure valid termination
    if (!is_line_end_or_comment(*line)) {
        return PROCESS_INVALID;
    }

    // Convert to int
    char *end_ptr;
    const long num = strtol(number, &end_ptr, 10);
    if (*end_ptr != '\0' || num < 0 || num >= 32768) return PROCESS_INVALID;
    *integer_literal = (int) num;

    return PROCESS_SUCCESS;
}

ProcessStatus process_a_instruction(const char **line, TokenTable *token_table) {
    if (!*line || !token_table) return PROCESS_ERROR;

    if (**line != '@') return PROCESS_ERROR;

    // Create '@' operator token
    Token *at_token = create_token(OPERATOR, OP_AT);
    if (!at_token || !token_table_add(token_table, at_token)) {
        free_token(at_token);
        return PROCESS_ERROR;
    }

    (*line)++;  // Move past '@'
    consume_whitespace(line);

    // Process integer literal:
    if (isdigit(**line)) {
        int integer_literal = 0;
        const ProcessStatus status = process_integer_literal(line, &integer_literal);
        if (status != PROCESS_SUCCESS) return status;

        // Tokenise integer literal
        Token *int_token = create_token(INTEGER_LITERAL, integer_literal);
        if (!int_token || !token_table_add(token_table, int_token)) {
            free_token(int_token);
            return PROCESS_ERROR;
        }
    } else {
        // Process symbol
        char buffer[MAX_LABEL_LEN + 1] = {0};
        const ProcessStatus status = process_symbol(line, buffer);
        if (status != PROCESS_SUCCESS) return status;

        // Tokenize symbol
        Token *symbol = create_token(SYMBOL, buffer);
        if (!symbol || !token_table_add(token_table, symbol)) {
            free_token(symbol);
            return PROCESS_ERROR;
        }
    }

    // Ensure only whitespace or comments remain
    if (!is_line_end_or_comment(*line)) {
        return PROCESS_INVALID;
    }

    // Tokenize newline
    Token *newline = create_token(NEWLINE, NULL);
    if (!newline || !token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

ProcessStatus process_c_instruction(const char **line, TokenTable *token_table) {
    // C-Instruction: -, +, &, |, !, ;, 0, 1, A, M, D, JGT, JEQ, JGE, JLT, JNE, JLE, JMP
    if (!*line || !token_table) return PROCESS_ERROR;

    consume_whitespace(line);
    const char *valid_chars = "-+&|!=;01ADMJGTENLQP";
    Token *token = NULL;
    while (strchr(valid_chars, **line)) {
        // if (isspace(**line)) {
        //     consume_whitespace(line);
        //     continue;
        // }
        if (is_line_end_or_comment(*line)) {
            // Tokenize newline
            Token *newline = create_token(NEWLINE, NULL);
            if (!newline || !token_table_add(token_table, newline)) {
                free_token(newline);
                return PROCESS_ERROR;
            }
            return PROCESS_SUCCESS;
        }

        // Process individual characters:
        switch (**line) {
            case '-':
                token = create_token(OPERATOR, OP_SUB);
                break;
            case '+':
                token = create_token(OPERATOR, OP_ADD);
                break;
            case '&':
                token = create_token(OPERATOR, OP_AND);
                break;
            case '|':
                token = create_token(OPERATOR, OP_OR);
                break;
            case '!':
                token = create_token(OPERATOR, OP_NOT);
                break;
            case '=':
                token = create_token(OPERATOR, OP_ASSIGN);
                break;
            case ';':
                token = create_token(SEPARATOR, SEP_SEMICOLON);
                break;
            case '0':
                token = create_token(INTEGER_LITERAL, 0);
                break;
            case '1':
                token = create_token(INTEGER_LITERAL, 1);
                break;
            case 'A':
                token = create_token(KEYWORD, KW_A);
                break;
            case 'D':
                token = create_token(KEYWORD, KW_D);
                break;
            case 'M':
                token = create_token(KEYWORD, KW_M);
                break;
            default: return PROCESS_INVALID;
        }

        if (!token | !token_table_add(token_table, token)) {
            free_token(token);
            return PROCESS_INVALID;
        }
        // If **line == ';' GOTO process Jump command
        if (**line == ';') {
            (*line)++;
            break;
        }
        (*line)++;
        consume_whitespace(line);
    }

    // Process JMP
    char buffer[4] = {0};
    int count = 0;
    while (!is_line_end_or_comment(*line) && count < 3) {
        consume_whitespace(line);
        buffer[count++] = **line;
        (*line)++;
    }

    // Should end with whitespace or comment
    if (!is_line_end_or_comment(*line)) {
        return PROCESS_INVALID;
    }

    // Check what JUMP statement has been used
    Keyword keyword_value;
    if (strcmp(buffer, "JGT") == 0) {
        keyword_value = KW_JGT;
    } else if (strcmp(buffer, "JEQ") == 0) {
        keyword_value = KW_JEQ;
    } else if (strcmp(buffer, "JGE") == 0) {
        keyword_value = KW_JGE;
    } else if (strcmp(buffer, "JLT") == 0) {
        keyword_value = KW_JLT;
    } else if (strcmp(buffer, "JNE") == 0) {
        keyword_value = KW_JNE;
    } else if (strcmp(buffer, "JLE") == 0) {
        keyword_value = KW_JLE;
    } else if (strcmp(buffer, "JMP") == 0) {
        keyword_value = KW_JMP;
    } else {
        return PROCESS_INVALID;
    }

    // Tokenise JUMP keyword
    token = create_token(KEYWORD, keyword_value);
    if (!token | !token_table_add(token_table, token)) {
        free_token(token);
        return PROCESS_INVALID;
    }

    // Tokenize newline
    Token *newline = create_token(NEWLINE, NULL);
    if (!newline || !token_table_add(token_table, newline)) {
        free_token(newline);
        return PROCESS_ERROR;
    }

    return PROCESS_SUCCESS;
}

