//
// Created by Alexander Fisher on 09/03/2025.
//

#include "token_table.h"
#include <stdlib.h>

// Internal struct definition (hidden from the user)
struct TokenTable {
    struct TokenNode *head;
    struct TokenNode *tail;
    struct TokenNode *current;
};

// Internal node structure (hidden from the user)
typedef struct TokenNode {
    Token *token;
    struct TokenNode *next;
} TokenNode;


TokenTable *token_table_create(void) {
    TokenTable *table = malloc(sizeof(TokenTable));
    if (!table) {
        perror("Failed to allocate TokenTable");
        return NULL;
    }
    table->head = table->tail = table->current = NULL;
    return table;
}

bool token_table_add(TokenTable *table, Token *token) {
    if (!table || !token) return false;

    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (!new_node) {
        perror("Failed to allocate TokenNode");
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

Token *token_table_next(TokenTable *table) {
    if (!table || !table->current) return NULL;

    Token *token = table->current->token;
    table->current = table->current->next;
    return token;
}

Token *token_table_peek(TokenTable *table) {
    if (!table || !table->current) return NULL;

    Token *token = table->current->token;
    return token;
}

void token_table_reset(TokenTable *table) {
    if (table) table->current = table->head;
}

void token_table_free(TokenTable *table) {
    if (!table) return;

    TokenNode *current = table->head;
    while (current) {
        TokenNode *next = current->next;
        free_token(current->token);
        free(current);
        current = next;
    }

    free(table);
}

void token_table_write_to_file(FILE *file, TokenTable *table) {
    if (!file || !table) return;

    token_table_reset(table);

    Token *token;
    while ((token = token_table_next(table)) != NULL) {
        char *token_str = token_to_str(token);
        fprintf(file, "%s\n", token_str);
        free(token_str);
    }
}
