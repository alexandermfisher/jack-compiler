//
// Created by Alexander Fisher on 04/03/2025.
//
#include <assert.h>
#include <stdio.h>
#include <symbol_table.h>
#include "config.h"


void test_symbol_table(void);
void test_load_symbol_table(void);

int main(void) {
    printf("Starting test_symbol_table...\n");
    test_symbol_table();
    test_load_symbol_table();
    printf("Test completed.\n");
    return 0;
}

void test_symbol_table(void) {
    // Create a new symbol table
    SymbolTable *table = symbol_table_create();
    assert(table != NULL);

    // Add symbols
    assert(symbol_table_add(table, "START", 100));
    assert(symbol_table_add(table, "LOOP", 200));
    assert(symbol_table_add(table, "END", 300));

    // Check if symbols exist
    assert(symbol_table_contains(table, "START"));
    assert(symbol_table_contains(table, "LOOP"));
    assert(symbol_table_contains(table, "END"));
    assert(!symbol_table_contains(table, "MISSING"));  // Should not exist

    // Retrieve symbol addresses
    assert(symbol_table_get_address(table, "START") == 100);
    assert(symbol_table_get_address(table, "LOOP") == 200);
    assert(symbol_table_get_address(table, "END") == 300);

    // Check retrieval of non-existent symbol (expected -1 or error value)
    assert(symbol_table_get_address(table, "MISSING") == -1);

    // Attempt to add a duplicate symbol (should fail)
    assert(!symbol_table_add(table, "START", 400));  // "START" already exists

    // Fill up the rest of symbol table to max capacity
    for (int i = 3; i < MAX_NUM_SYMBOLS; i++) {
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "SYM_%d", i);
        symbol_table_add(table, buffer, i * 10);
    }

    // Adding one more should fail (table is full)
    assert(!symbol_table_add(table, "EXTRA", 9999));

    // Cleanup
    symbol_table_free(table);

    printf("    ✅ test_symbol_table passed!\n");
}

void test_load_symbol_table(void) {
    // Create a new symbol table
    SymbolTable *table = symbol_table_create();
    assert(table != NULL);

    // Load predefined symbols into the table
    assert(load_predefined_symbols(table));

    // Check if predefined symbols exist
    assert(symbol_table_contains(table, "SP"));
    assert(symbol_table_contains(table, "LCL"));
    assert(symbol_table_contains(table, "ARG"));
    assert(symbol_table_contains(table, "THIS"));
    assert(symbol_table_contains(table, "THAT"));
    assert(symbol_table_contains(table, "R0"));
    assert(symbol_table_contains(table, "R1"));
    assert(symbol_table_contains(table, "R2"));
    assert(symbol_table_contains(table, "R3"));
    assert(symbol_table_contains(table, "R4"));
    assert(symbol_table_contains(table, "R5"));
    assert(symbol_table_contains(table, "R6"));
    assert(symbol_table_contains(table, "R7"));
    assert(symbol_table_contains(table, "R8"));
    assert(symbol_table_contains(table, "R9"));
    assert(symbol_table_contains(table, "R10"));
    assert(symbol_table_contains(table, "R11"));
    assert(symbol_table_contains(table, "R12"));
    assert(symbol_table_contains(table, "R13"));
    assert(symbol_table_contains(table, "R14"));
    assert(symbol_table_contains(table, "R15"));
    assert(symbol_table_contains(table, "SCREEN"));
    assert(symbol_table_contains(table, "KBD"));

    // Retrieve and check addresses for a few symbols
    assert(symbol_table_get_address(table, "SP") == 0);
    assert(symbol_table_get_address(table, "LCL") == 1);
    assert(symbol_table_get_address(table, "ARG") == 2);
    assert(symbol_table_get_address(table, "THIS") == 3);
    assert(symbol_table_get_address(table, "THAT") == 4);
    assert(symbol_table_get_address(table, "R0") == 0);
    assert(symbol_table_get_address(table, "R1") == 1);
    assert(symbol_table_get_address(table, "R2") == 2);
    assert(symbol_table_get_address(table, "R3") == 3);

    // Ensure no duplicate symbols were added
    assert(symbol_table_add(table, "SP", 0) == false);  // "SP" already exists
    assert(symbol_table_add(table, "R0", 0) == false);  // "R0" already exists

    // Cleanup
    symbol_table_free(table);

    printf("    ✅ test_load_symbol_table passed!\n");
}
