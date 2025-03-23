#include "assembler.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "code_generator.h"
#include "token.h"
#include <token_table.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Internal full definition of Assembler
struct Assembler {
    AssemblerConfig config;
    // Add internal state here like:
    TokenTable *token_table;
    SymbolTable *symbol_table;
    Parser *parser;
    // Any other internal state
};

Assembler *assembler_create(const AssemblerConfig *config) {
    if (!config) return NULL;

    // Validate required fields (file pointers and file paths)
    if (!config->source_asm || !config->target_hack ||
        !config->source_filepath || !config->target_filepath) {
        return NULL;
        }

    // Validate that file pointers are accessible (optional read/write test)
    if (ferror(config->source_asm) || ferror(config->target_hack)) {
        return NULL;
    }

    Assembler *assembler = calloc(1, sizeof(Assembler));
    if (!assembler) return NULL;

    assembler->config.source_asm = config->source_asm;
    assembler->config.source_filepath = config->source_filepath;
    assembler->config.target_hack = config->target_hack;
    assembler->config.target_filepath = config->target_filepath;
    assembler->config.token_output_filepath = config->token_output_filepath;
    assembler->config.print_tokens = config->print_tokens ? true : false;
    assembler->config.logger = config->logger ? config->logger : NULL;

    // Create TokenTable
    assembler->token_table = token_table_create((TokenFreeFunc)free_token, (TokenToStr)token_to_str);
    if (!assembler->token_table) {
        free(assembler);
        return NULL;
    }

    // Create SymbolTable
    assembler->symbol_table = symbol_table_create();
    if (!assembler->symbol_table) {
        token_table_free(assembler->token_table);
        free(assembler);
        return NULL;
    }

    // Load predefined symbols into symbol table
    if (!load_predefined_symbols(assembler->symbol_table)) {
        token_table_free(assembler->token_table);
        symbol_table_free(assembler->symbol_table);
        free(assembler);
        return NULL;
    }

    // Create Parser:
    assembler->parser = parser_create(assembler->token_table, assembler->symbol_table);
    if (!assembler->parser) {
        token_table_free(assembler->token_table);
        symbol_table_free(assembler->symbol_table);
        free(assembler);
        return NULL;
    }

    return assembler;
}


int run_assembler(FILE *source_asm, char *source_filepath, FILE *target_hack, const bool print_tokens) {
    if (!source_asm || !target_hack || !source_filepath) return 1;

    int return_status = 0;
    FILE *token_lex_file = NULL;
    TokenTable *token_table = NULL;
    SymbolTable *symbol_table = NULL;
    Parser *parser = NULL;

    // Create and initialise token table, and symbol table
    token_table = token_table_create((TokenFreeFunc)free_token, (TokenToStr)token_to_str);
    if (!token_table) return 1;
    symbol_table = symbol_table_create();
    if (!symbol_table) {
        token_table_free(token_table);
        return 1;
    }

    // Load predefined symbols into symbol table
    if (!load_predefined_symbols(symbol_table)) {
        return_status = 1;
        goto cleanup;
    }

    // First Pass - tokenise line by line and populate symbol table with labels
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rom_address = 0;
    int line_num = 1;
    while ((read=getline(&line, &len, source_asm)) != -1) {
        const ProcessStatus status = lex_line(line, read, token_table, symbol_table, &rom_address);
        if (status != PROCESS_SUCCESS) {
            if (status == PROCESS_INVALID) {
                fprintf(stderr, "%s:%d: syntax error: unable to process line - %s\n",
                    source_filepath, line_num, line);
            }
            return_status = 1;
            free(line);
            goto cleanup;
        }
        line_num++;
    }
    if (line) free(line);
    token_table_reset(token_table);

    // Initialise Parser
    parser = parser_create(token_table, symbol_table);
    if (!parser) {
        return_status = 1;
        goto cleanup;
    }

    // Second Pass - Code Generation
    int ram_address = 16;
    while (parser_has_more_commands(parser)) {
        if (!advance(parser)) break;
        if (parser->instruction->type == L_INSTRUCTION) continue;
        if (parser->instruction->type == A_INSTRUCTION_SYMBOL) {
            if (!symbol_table_contains(symbol_table, parser->instruction->symbol)) {
                symbol_table_add(symbol_table, parser->instruction->symbol, ram_address++);
            }
            parser->instruction->value = symbol_table_get_address(symbol_table, parser->instruction->symbol);
            parser->instruction->type = A_INSTRUCTION_VALUE;
        }

        // Generate binary instruction
        char binary_instruction[17] = {0};
        generate_binary(parser->instruction, binary_instruction);

        // Write to the .hack file
        fprintf(target_hack, "%s\n", binary_instruction);
    }


    cleanup:
    // Print token table to file if requested
    if (print_tokens) {
        // Open file for token.lex
        token_lex_file = fopen("tokens.lex", "w");
        if (!token_lex_file) {
            perror("Error: Failed to open 'tokens.lex'");
            return_status = 1;
        } else {
            token_table_write_to_file(token_lex_file, token_table);
            fclose(token_lex_file);
        }
    }
    if (parser) parser_free(parser);
    token_table_free(token_table);
    symbol_table_free(symbol_table);

    return return_status;  // Return 0 on success, 1 on error
}
