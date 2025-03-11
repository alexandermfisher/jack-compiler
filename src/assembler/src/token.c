//
// Created by alexanderfisher on 08/03/25.
//
#include "token.h"
#include "config.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char *token_type_to_str(const TokenType type) {
    switch (type) {
        case SYMBOL: return "SYMBOL";
        case KEYWORD: return "KEYWORD";
        case INTEGER_LITERAL: return "INTEGER_LITERAL";
        case OPERATOR: return "OPERATOR";
        case SEPARATOR: return "SEPARATOR";
        case NEWLINE: return "NEWLINE";
        default: return "INVALID";
    }
}

const char *keyword_to_str(const Keyword keyword) {
    switch (keyword) {
        case KW_A: return "A";
        case KW_D: return "D";
        case KW_M: return "M";
        case KW_JGT: return "JGT";
        case KW_JEQ: return "JEQ";
        case KW_JGE: return "JGE";
        case KW_JLT: return "JLT";
        case KW_JNE: return "JNE";
        case KW_JLE: return "JLE";
        case KW_JMP: return "JMP";
        default: return "UNKNOWN_KEYWORD";
    }
}

const char *operator_to_str(const Operator op) {
    switch (op) {
        case OP_AT: return "@";
        case OP_ASSIGN: return "=";
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_AND: return "&";
        case OP_OR: return "|";
        case OP_NOT: return "!";
        default: return "UNKNOWN_OPERATOR";
    }
}

const char *separator_to_str(const Separator sep) {
    switch (sep) {
        case SEP_SEMICOLON: return ";";
        case SEP_LPAREN: return "(";
        case SEP_RPAREN: return ")";
        default: return "UNKNOWN_SEPARATOR";
    }
}

void free_token(Token *token) {
    if (!token) return;

    if (token->type == SYMBOL && token->value.symbol) {
        free(token->value.symbol);
    }
    free(token);
}

char *token_to_str(const Token *token) {
    if (!token) return NULL;

    // Ensure buffer size accounts for longest possible SYMBOL + formatting
    const size_t buffer_size = MAX_LABEL_LEN + 32;
    char *buffer = malloc(buffer_size);
    if (!buffer) return NULL;

    switch (token->type) {
        case KEYWORD:
            snprintf(buffer, buffer_size, "KEYWORD %s", keyword_to_str(token->value.keyword));
        break;
        case OPERATOR:
            snprintf(buffer, buffer_size, "OPERATOR %s", operator_to_str(token->value.operator));
        break;
        case SEPARATOR:
            snprintf(buffer, buffer_size, "SEPARATOR %s", separator_to_str(token->value.separator));
        break;
        case INTEGER_LITERAL:
            snprintf(buffer, buffer_size, "INTEGER_LITERAL %d", token->value.integer);
        break;
        case SYMBOL:
            snprintf(buffer, buffer_size, "SYMBOL %s", token->value.symbol);
        break;
        case NEWLINE:
            snprintf(buffer, buffer_size, "NEWLINE");
        break;
        case INVALID:
            snprintf(buffer, buffer_size, "INVALID %s", token->value.symbol);
        break;
        default:
            snprintf(buffer, buffer_size, "UNKNOWN TOKEN TYPE");
        break;
    }

    return buffer;
}

Token *create_token(TokenType type, ...) {
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;

    token->type = type;
    va_list args;
    va_start(args, type);

    switch (type) {
        case SYMBOL: {
            char *symbol = va_arg(args, char *);
            if (!symbol) { free(token); return NULL; }
            token->value.symbol = strdup(symbol);
            break;
        }
        case INTEGER_LITERAL: {
            const int num = va_arg(args, int);
            token->value.integer = num;
            break;
        }
        case KEYWORD: {
            const Keyword kw = va_arg(args, Keyword);
            token->value.keyword = kw;
            break;
        }
        case OPERATOR: {
            const Operator op = va_arg(args, Operator);
            token->value.operator = op;
            break;
        }
        case SEPARATOR: {
            const Separator sep = va_arg(args, Separator);
            token->value.separator = sep;
            break;
        }
        case NEWLINE:
        case INVALID:
            // No additional arguments expected for these cases
            token->value.symbol = NULL;
        break;
        default:
            free(token);
        token = NULL;
        break;
    }

    va_end(args);
    return token;
}

