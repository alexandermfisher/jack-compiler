#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include "line_processor.h"
#include "symbol_table.h"
#include "token_table.h"


int run_assembler(FILE *source_asm, char *source_filepath, FILE *target_hack) {
    // validate file extensions:
    if (!source_asm || !target_hack || !source_filepath) {
        return EXIT_FAILURE;
    }

    // Create and initialise token table, and symbol table
    TokenTable *token_table = token_table_create();
    if (!token_table) return 1;

    SymbolTable *symbol_table = symbol_table_create();
    if (!symbol_table) {
        token_table_free(token_table);
        return 1;
    }

    // Load predefined symbols into symbol table
    if (!load_predefined_symbols(symbol_table)) {
        token_table_free(token_table);
        symbol_table_free(symbol_table);
        return 1;
    }

    // First Pass - tokenise line by line and populate symbol table with labels
    FILE *token_lex_file = fopen("tokens.lex", "w");
    if (!token_lex_file) {
        perror("Error: Failed to open 'tokens.lex'");
        token_table_free(token_table);
        symbol_table_free(symbol_table);
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    int rom_address = 0;
    int line_num = 1;
    while (getline(&line, &len, source_asm) != -1) {
        printf("%s", line);
        const int res = process_line(line, token_table, symbol_table, &rom_address);
        if (res != PROCESS_SUCCESS) {
            if (res == PROCESS_INVALID) {
                fprintf(stderr, "%s:%d: syntax error: unable to process line - %s\n", source_filepath, line_num, line);
            }

            // Write token_table to tokens.lex
            token_table_write_to_file(token_lex_file, token_table);
            fclose(token_lex_file);

            // Free tables
            token_table_free(token_table);
            symbol_table_free(symbol_table);
            free(line);
            return 1;
        }
        line_num++;
    }
    if (line) free(line);

    // Initialise Parser

    // Second Pass - code generation

    // Write token_table to tokens.lex
    token_table_write_to_file(token_lex_file, token_table);
    fclose(token_lex_file);

    token_table_free(token_table);
    symbol_table_free(symbol_table);
    return 0;
}

