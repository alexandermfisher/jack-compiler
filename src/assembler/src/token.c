//
// Created by alexanderfisher on 08/03/25.
//
#include "token.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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

const char *operator_to_string(const Operator op) {
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

const char *separator_to_string(const Separator sep) {
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

void write_token(FILE *file, const Token token) {
    switch (token.type) {
        case KEYWORD:
            fprintf(file, "KEYWORD %d\n", token.value.keyword);
        break;
        case OPERATOR:
            fprintf(file, "OPERATOR %d\n", token.value.operator);
        break;
        case SEPARATOR:
            fprintf(file, "SEPARATOR %d\n", token.value.separator);
        break;
        case INTEGER_LITERAL:
            fprintf(file, "INTEGER_LITERAL %d\n", token.value.integer);
        break;
        case SYMBOL:
            fprintf(file, "SYMBOL %s\n", token.value.symbol);
        break;
        case NEWLINE:
            fprintf(file, "NEWLINE\n");
        break;
        case INVALID:
            fprintf(file, "INVALID %s\n", token.value.symbol);
        break;
    }
}

Token *create_token(TokenType type, ...) {
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;  // Allocation failure

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










// Token *create_token(const TokenType type, void *value) {
//     if (type != NEWLINE && type != INVALID && !value) {
//         return NULL;
//     }
//
//     Token *token = malloc(sizeof(Token));
//     if (!token) {
//         perror("Failed to allocate memory for token");
//         return NULL;
//     }
//
//     token->type = type;
//
//     switch (type) {
//         case SYMBOL:
//             token->value.symbol = strdup(value);
//         if (!token->value.symbol) {
//             perror("Failed to allocate memory for symbol");
//             free(token);
//             return NULL;
//         }
//         break;
//
//         case INTEGER_LITERAL:
//             token->value.integer = *(int *)value;
//         break;
//
//         case KEYWORD:
//             token->value.keyword = *(Keyword *)value;
//         break;
//
//         case OPERATOR:
//             token->value.operator = *(Operator *)value;
//         break;
//
//         case SEPARATOR:
//             token->value.separator = *(Separator *)value;
//         break;
//
//         case NEWLINE:
//         case INVALID:
//             token->value.symbol = NULL; // No additional data needed
//         break;
//
//         default:
//             fprintf(stderr, "Invalid token type\n");
//         free(token);
//         return NULL;
//     }
//
//     return token;
// }


