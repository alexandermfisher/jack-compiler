//
// Created by Alexander Fisher on 04/03/2025.
//
#include <assert.h>
#include <stdio.h>
#include <symbol_table.h>

#define MAX_NUM_SYMBOLS 100

void test_symbol_table(void);


int main(void) {
    printf("Starting test_symbol_table...\n");
    test_symbol_table();
    printf("Test completed.\n");
    return 0;
}

void test_symbol_table(void) {
    printf("Running test_symbol_table...\n");

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
