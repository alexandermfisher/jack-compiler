//
// Created by alexanderfisher on 08/03/25.
//
#include "token.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Frees token
void free_token(Token *token) {
    if (!token) return;

    if (token->type == TOKEN_SYMBOL && token->value.symbol) {
        free(token->value.symbol);
    }
    free(token);
}

// Creates token
Token *create_token(const TokenType type, ...) {
    Token *token = malloc(sizeof(Token));
    if (!token) return NULL;

    token->type = type;
    token->value.symbol = NULL;

    if (type == TOKEN_SYMBOL || type == TOKEN_INTEGER) {
        va_list args;
        va_start(args, type);

        if (type == TOKEN_SYMBOL) {
            char *symbol = va_arg(args, char *);
            if (!symbol) {
                free(token);
                va_end(args);
                return NULL;
            }
            token->value.symbol = strdup(symbol);
            if (!token->value.symbol) { // Handle strdup failure
                free(token);
                va_end(args);
                return NULL;
            }
        } else {
            token->value.integer = va_arg(args, int);
        }

        va_end(args);
    }

    return token;
}

// Function to convert a token to a string representation
char *token_to_str(const Token *token) {
    if (!token) return NULL;

    char *result = NULL;

    switch (token->type) {
        // A-instruction tokens
        case TOKEN_AT:
            asprintf(&result, "TOKEN_AT @");
            break;
        case TOKEN_SYMBOL:
            asprintf(&result, "TOKEN_SYMBOL %s", token->value.symbol);
            break;
        case TOKEN_INTEGER:
            asprintf(&result, "TOKEN_INTEGER %d", token->value.integer);
            break;

        // L-instruction tokens
        case TOKEN_LPAREN:
            asprintf(&result, "TOKEN_LPAREN (");
            break;
        case TOKEN_RPAREN:
            asprintf(&result, "TOKEN_RPAREN )");
            break;

        // Destinations
        case TOKEN_DEST_M:
            asprintf(&result, "TOKEN_DEST M");
            break;
        case TOKEN_DEST_D:
            asprintf(&result, "TOKEN_DEST D");
            break;
        case TOKEN_DEST_MD:
            asprintf(&result, "TOKEN_DEST MD");
            break;
        case TOKEN_DEST_A:
            asprintf(&result, "TOKEN_DEST A");
            break;
        case TOKEN_DEST_AM:
            asprintf(&result, "TOKEN_DEST AM");
            break;
        case TOKEN_DEST_AD:
            asprintf(&result, "TOKEN_DEST AD");
            break;
        case TOKEN_DEST_AMD:
            asprintf(&result, "TOKEN_DEST AMD");
            break;

        // Computation mnemonics
        case TOKEN_COMP_0:
            asprintf(&result, "TOKEN_COMP 0");
            break;
        case TOKEN_COMP_1:
            asprintf(&result, "TOKEN_COMP 1");
            break;
        case TOKEN_COMP_NEG1:
            asprintf(&result, "TOKEN_COMP -1");
            break;
        case TOKEN_COMP_D:
            asprintf(&result, "TOKEN_COMP D");
            break;
        case TOKEN_COMP_A:
            asprintf(&result, "TOKEN_COMP A");
            break;
        case TOKEN_COMP_M:
            asprintf(&result, "TOKEN_COMP M");
            break;
        case TOKEN_COMP_NOT_D:
            asprintf(&result, "TOKEN_COMP !D");
            break;
        case TOKEN_COMP_NOT_A:
            asprintf(&result, "TOKEN_COMP !A");
            break;
        case TOKEN_COMP_NOT_M:
            asprintf(&result, "TOKEN_COMP !M");
            break;
        case TOKEN_COMP_NEG_D:
            asprintf(&result, "TOKEN_COMP -D");
            break;
        case TOKEN_COMP_NEG_A:
            asprintf(&result, "TOKEN_COMP -A");
            break;
        case TOKEN_COMP_NEG_M:
            asprintf(&result, "TOKEN_COMP -M");
            break;
        case TOKEN_COMP_DPLUS1:
            asprintf(&result, "TOKEN_COMP D+1");
            break;
        case TOKEN_COMP_APLUS1:
            asprintf(&result, "TOKEN_COMP A+1");
            break;
        case TOKEN_COMP_MPLUS1:
            asprintf(&result, "TOKEN_COMP M+1");
            break;
        case TOKEN_COMP_DMINUS1:
            asprintf(&result, "TOKEN_COMP D-1");
            break;
        case TOKEN_COMP_AMINUS1:
            asprintf(&result, "TOKEN_COMP A-1");
            break;
        case TOKEN_COMP_MMINUS1:
            asprintf(&result, "TOKEN_COMP M-1");
            break;
        case TOKEN_COMP_DPLUSA:
            asprintf(&result, "TOKEN_COMP D+A");
            break;
        case TOKEN_COMP_DPLUSM:
            asprintf(&result, "TOKEN_COMP D+M");
            break;
        case TOKEN_COMP_DMINUSA:
            asprintf(&result, "TOKEN_COMP D-A");
            break;
        case TOKEN_COMP_DMINUSM:
            asprintf(&result, "TOKEN_COMP D-M");
            break;
        case TOKEN_COMP_AMINUSD:
            asprintf(&result, "TOKEN_COMP A-D");
            break;
        case TOKEN_COMP_MMINUSD:
            asprintf(&result, "TOKEN_COMP M-D");
            break;
        case TOKEN_COMP_DANDA:
            asprintf(&result, "TOKEN_COMP D&A");
            break;
        case TOKEN_COMP_DANDM:
            asprintf(&result, "TOKEN_COMP D&M");
            break;
        case TOKEN_COMP_DORA:
            asprintf(&result, "TOKEN_COMP D|A");
            break;
        case TOKEN_COMP_DORM:
            asprintf(&result, "TOKEN_COMP D|M");
            break;

        // Jump mnemonics
        case TOKEN_JUMP_JGT:
            asprintf(&result, "TOKEN_JUMP JGT");
            break;
        case TOKEN_JUMP_JEQ:
            asprintf(&result, "TOKEN_JUMP JEQ");
            break;
        case TOKEN_JUMP_JGE:
            asprintf(&result, "TOKEN_JUMP JGE");
            break;
        case TOKEN_JUMP_JLT:
            asprintf(&result, "TOKEN_JUMP JLT");
            break;
        case TOKEN_JUMP_JNE:
            asprintf(&result, "TOKEN_JUMP JNE");
            break;
        case TOKEN_JUMP_JLE:
            asprintf(&result, "TOKEN_JUMP JLE");
            break;
        case TOKEN_JUMP_JMP:
            asprintf(&result, "TOKEN_JUMP JMP");
            break;

        // Separators
        case TOKEN_EQUALS:
            asprintf(&result, "TOKEN_EQUALS =");
            break;
        case TOKEN_SEMICOLON:
            asprintf(&result, "TOKEN_SEMICOLON ;");
            break;

        // Special cases
        case TOKEN_EOF:
            asprintf(&result, "TOKEN_EOF");
            break;
        case TOKEN_INVALID:
            asprintf(&result, "TOKEN_INVALID");
            break;
        case NEWLINE:
            asprintf(&result, "NEWLINE");
            break;
        default:
            asprintf(&result, "UNKNOWN TOKEN TYPE '%d'", token->type);
            break;
    }

    return result;
}
