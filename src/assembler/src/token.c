//
// Created by alexanderfisher on 08/03/25.
//

#include "token.h"
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
        default: return "UNKNOWN_TOKEN_TYPE";
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

TokenType str_to_token_type(const char *str) {
    if (strcmp(str, "SYMBOL") == 0) return SYMBOL;
    if (strcmp(str, "KEYWORD") == 0) return KEYWORD;
    if (strcmp(str, "INTEGER_LITERAL") == 0) return INTEGER_LITERAL;
    if (strcmp(str, "OPERATOR") == 0) return OPERATOR;
    if (strcmp(str, "SEPARATOR") == 0) return SEPARATOR;
    if (strcmp(str, "NEWLINE") == 0) return NEWLINE;
    return -1;
}

Keyword str_to_keyword(const char *str) {
    if (strcmp(str, "A") == 0) return KW_A;
    if (strcmp(str, "D") == 0) return KW_D;
    if (strcmp(str, "M") == 0) return KW_M;
    if (strcmp(str, "JGT") == 0) return KW_JGT;
    if (strcmp(str, "JEQ") == 0) return KW_JEQ;
    if (strcmp(str, "JGE") == 0) return KW_JGE;
    if (strcmp(str, "JLT") == 0) return KW_JLT;
    if (strcmp(str, "JNE") == 0) return KW_JNE;
    if (strcmp(str, "JLE") == 0) return KW_JLE;
    if (strcmp(str, "JMP") == 0) return KW_JMP;
    return -1;
}

Operator str_to_operator(const char *str) {
    if (strcmp(str, "@") == 0) return OP_AT;
    if (strcmp(str, "=") == 0) return OP_ASSIGN;
    if (strcmp(str, "+") == 0) return OP_ADD;
    if (strcmp(str, "-") == 0) return OP_SUB;
    if (strcmp(str, "!") == 0) return OP_NOT;
    if (strcmp(str, "&") == 0) return OP_AND;
    if (strcmp(str, "|") == 0) return OP_OR;
    return -1;
}

Separator str_to_separator(const char *str) {
    if (strcmp(str, ";") == 0) return SEP_SEMICOLON;
    if (strcmp(str, "(") == 0) return SEP_LPAREN;
    if (strcmp(str, ")") == 0) return SEP_RPAREN;
    return -1;
}

Token *malloc_token(void) {
    Token *token = malloc(sizeof(Token));
    return token;
}

void free_token(Token *token) {
    free(token);
}



