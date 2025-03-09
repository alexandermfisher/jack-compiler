#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_NUM_SYMBOLS 100

// Internal structure for a symbol entry
typedef struct {
    char *symbol;
    int address;
} SymbolEntry;

// Internal structure for the symbol table
struct SymbolTable {
    SymbolEntry entries[MAX_NUM_SYMBOLS];
    size_t count;
};

// Create a new symbol table
SymbolTable *symbol_table_create(void) {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    if (!table) {
        perror("Failed to allocate symbol table");
        return NULL;
    }
    table->count = 0;
    return table;
}

// Free the symbol table
void symbol_table_free(SymbolTable *table) {
    if (!table) return;

    // Free each symbol string
    for (size_t i = 0; i < table->count; i++) {
        free(table->entries[i].symbol);
    }
    free(table);
}

// Add a new symbol to the table
bool symbol_table_add(SymbolTable *table, const char *symbol, int address) {
    if (!table || !symbol) return false;

    // Check if the table is full
    if (table->count >= MAX_NUM_SYMBOLS) {
        fprintf(stderr, "Error: Maximum number of symbols (%d) reached. Cannot add symbol '%s'.\n",
                MAX_NUM_SYMBOLS, symbol);
        return false;
    }

    // Check if the symbol already exists
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].symbol, symbol) == 0) {
            return false; // Symbol already exists
        }
    }

    // Allocate and copy the symbol name
    table->entries[table->count].symbol = strdup(symbol);
    if (!table->entries[table->count].symbol) {
        perror("Failed to allocate memory for symbol");
        return false;
    }

    table->entries[table->count].address = address;
    table->count++;
    return true;
}

// Check if a symbol exists in the table
bool symbol_table_contains(SymbolTable *table, const char *symbol) {
    if (!table || !symbol) return false;
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].symbol, symbol) == 0) {
            return true;
        }
    }
    return false;
}

// Get the address associated with a symbol
int symbol_table_get_address(SymbolTable *table, const char *symbol) {
    if (!table || !symbol) return -1;
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].symbol, symbol) == 0) {
            return table->entries[i].address;
        }
    }
    return -1; // Not found
}
