#include "symbol_table.h"

#include <logger.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PREDEFINED_COUNT 23

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
    if (!table) return NULL;
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
bool symbol_table_add(SymbolTable *table, const char *symbol, const int address) {
    if (!table || !symbol) return false;

    if (symbol_table_contains(table, symbol)) return false;

    // Check if the table is full
    if (table->count >= MAX_NUM_SYMBOLS) {
        GLOG(LOG_ERROR, "Symbol table capacity exceeded (max %d symbols)", MAX_NUM_SYMBOLS);
        return false;
    }

    // Allocate and copy the symbol name
    table->entries[table->count].symbol = strdup(symbol);
    if (!table->entries[table->count].symbol) {
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

// Function to load predefined symbols into the symbol table
bool load_predefined_symbols(SymbolTable *table) {
    if (!table) return false;

    const struct {
        const char *symbol;
        int address;
    } predefined_symbols[PREDEFINED_COUNT] = {
        {"SP", 0},      {"LCL", 1},
        {"ARG", 2},   {"THIS", 3},
        {"THAT", 4},{"R0", 0},
        {"R1", 1},    {"R2", 2},
        {"R3", 3},   {"R4", 4},
        {"R5", 5},      {"R6", 6},
        {"R7", 7},    {"R8", 8},
        {"R9", 9},{"R10", 10},
        {"R11", 11},  {"R12", 12},
        {"R13", 13}, {"R14", 14},
        {"R15", 15},    {"SCREEN", 16384},
        {"KBD", 24576}
    };

    // Add all predefined symbols to the symbol table
    for (int i = 0; i < PREDEFINED_COUNT; i++) {
        if (!symbol_table_add(table, predefined_symbols[i].symbol, predefined_symbols[i].address)) {
            return false;
        }
    }
    return true;
}
