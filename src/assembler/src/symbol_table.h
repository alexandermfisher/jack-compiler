//
// Created by alexanderfisher on 08/03/25.
//
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

#define MAX_NUM_SYMBOLS 1000

// Declare SymbolTable as an opaque type
typedef struct SymbolTable SymbolTable;

/**
 * Creates and initializes a new SymbolTable instance.
 *
 * @return Pointer to the newly created SymbolTable.
 */
SymbolTable *symbol_table_create(void);

/**
 * Frees the SymbolTable and any resources it holds.
 *
 * @param table Pointer to the SymbolTable to free.
 */
void symbol_table_free(SymbolTable *table);

/**
 * Adds a new (symbol, address) pair to the SymbolTable.
 *
 * @param table Pointer to the SymbolTable.
 * @param symbol The symbol (string) to add.
 * @param address The associated address of the symbol.
 * @return true if the symbol was successfully added, false if the table is full or symbol already exists.
 */
bool symbol_table_add(SymbolTable *table, const char *symbol, int address);

/**
 * Checks whether a given symbol exists in the SymbolTable.
 *
 * @param table Pointer to the SymbolTable.
 * @param symbol The symbol (string) to search for.
 * @return true if the symbol exists, false otherwise.
 */
bool symbol_table_contains(SymbolTable *table, const char *symbol);

/**
 * Retrieves the address associated with a given symbol.
 *
 * @param table Pointer to the SymbolTable.
 * @param symbol The symbol (string) whose address is requested.
 * @return The address of the symbol if found, or -1 if the symbol does not exist.
 */
int symbol_table_get_address(SymbolTable *table, const char *symbol);

/**
 * Loads predefined symbols into the SymbolTable.
 *
 * Typically used to populate symbols like SP, LCL, ARG, THIS, THAT, R0-R15, SCREEN, KBD, etc.
 *
 * @param table Pointer to the SymbolTable.
 * @return true if predefined symbols were successfully loaded, false otherwise.
 */
bool load_predefined_symbols(SymbolTable *table);

#endif // SYMBOL_TABLE_H


