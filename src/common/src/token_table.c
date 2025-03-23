// Created by Alexander Fisher on 09/03/2025.

#include "token_table.h"
#include <stdlib.h>

// Internal node structure (hidden from user)
typedef struct TokenNode {
    void *token;
    struct TokenNode *next;
} TokenNode;

// Internal struct definition (hidden from user)
struct TokenTable {
    TokenNode *head;
    TokenNode *tail;
    TokenNode *current;

    TokenFreeFunc free_func;
    TokenToStr token_to_str;
};

TokenTable *token_table_create(const TokenFreeFunc free_func, const TokenToStr token_to_str) {
    TokenTable *table = malloc(sizeof(TokenTable));
    if (!table) {
        return NULL;
    }
    table->head = table->tail = table->current = NULL;
    table->free_func = free_func;
    table->token_to_str = token_to_str;
    return table;
}

bool token_table_add(TokenTable *table, void *token) {
    if (!table || !token) return false;

    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (!new_node) {
        return false;
    }

    new_node->token = token;
    new_node->next = NULL;

    if (table->tail) {
        table->tail->next = new_node;
    } else {
        table->head = new_node;
    }

    table->tail = new_node;
    return true;
}

void *token_table_next(TokenTable *table) {
    if (!table || !table->current) return NULL;

    void *token = table->current->token;
    table->current = table->current->next;
    return token;
}

void *token_table_peek(TokenTable *table) {
    if (!table || !table->current) return NULL;
    return table->current->token;
}

void token_table_reset(TokenTable *table) {
    if (table) table->current = table->head;
}

void token_table_free(TokenTable *table) {
    if (!table) return;

    TokenNode *current = table->head;
    while (current) {
        TokenNode *next = current->next;
        if (table->free_func) {
            table->free_func(current->token);  // Use user-supplied free function
        }
        free(current);
        current = next;
    }

    free(table);
}

void token_table_write_to_file(FILE *file, TokenTable *table) {
    if (!file || !table || !table->token_to_str) return;

    token_table_reset(table);

    void *token;
    while ((token = token_table_next(table)) != NULL) {
        char *token_str = table->token_to_str(token);
        fprintf(file, "%s\n", token_str);
        free(token_str);
    }
}
