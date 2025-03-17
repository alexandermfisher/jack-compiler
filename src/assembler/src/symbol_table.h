//
// Created by alexanderfisher on 08/03/25.
//
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdbool.h>

#define MAX_NUM_SYMBOLS 1000

// Declare SymbolTable as an opaque type
typedef struct SymbolTable SymbolTable;

// Create a new symbol table
SymbolTable *symbol_table_create(void);

// Free the symbol table
void symbol_table_free(SymbolTable *table);

// Add a (symbol, address) pair to the table
bool symbol_table_add(SymbolTable *table, const char *symbol, int address);

// Check if the table contains a given symbol
bool symbol_table_contains(SymbolTable *table, const char *symbol);

// Get the address associated with a symbol
int symbol_table_get_address(SymbolTable *table, const char *symbol);

// Function to load predefined symbols into the symbol table
bool load_predefined_symbols(SymbolTable *table);

#endif // SYMBOL_TABLE_H

