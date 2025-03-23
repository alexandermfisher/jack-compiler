#include "assembler.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "code_generator.h"
#include "token.h"
#include <logger.h>
#include <token_table.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Internal full definition of Assembler
struct Assembler {
    AssemblerConfig config;
    TokenTable *token_table;
    SymbolTable *symbol_table;
    Parser *parser;
};

Assembler *assembler_create(const AssemblerConfig *config) {
    if (!config) return NULL;

    // Validate required fields (file pointers and file paths)
    if (!config->source_asm || !config->target_hack ||
        !config->source_filepath || !config->target_filepath) {
        return NULL;
    }

    Assembler *assembler = calloc(1, sizeof(Assembler));
    if (!assembler) return NULL;

    assembler->config.source_asm = config->source_asm;
    assembler->config.source_filepath = config->source_filepath;
    assembler->config.target_hack = config->target_hack;
    assembler->config.target_filepath = config->target_filepath;
    assembler->config.token_output = config->token_output;

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

void assembler_free(Assembler *assembler) {
    if (!assembler) return;

    // Free the parser if it was created
    if (assembler->parser) {
        parser_free(assembler->parser);
    }

    // Free the token table
    if (assembler->token_table) {
        token_table_free(assembler->token_table);
    }

    // Free the symbol table
    if (assembler->symbol_table) {
        symbol_table_free(assembler->symbol_table);
    }

    // Finally, free the assembler struct itself
    free(assembler);
}

int assembler_assemble(Assembler *assembler) {
    if (!assembler) return 1;

    int return_status = 0;

    // First Pass - Tokenize lines and populate symbol table with labels
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rom_address = 0;
    int line_num = 1;
    while ((read = getline(&line, &len, assembler->config.source_asm)) != -1) {
        const ProcessStatus status = lex_line(line, read, assembler->token_table,
                                        assembler->symbol_table, &rom_address);
        if (status != PROCESS_SUCCESS) {
            if (status == PROCESS_INVALID) {
                GLOG(LOG_ERROR, "%s:%d: syntax error: unable to process line - %s",
                     assembler->config.source_filepath, line_num, line);
            } else if (status == PROCESS_ERROR) {
                GLOG(LOG_ERROR, "%s:%d: internal error (memory/system failure) while processing line.",
                     assembler->config.source_filepath, line_num);
            }
            free(line);
            return_status = 1;
            goto end;
        }
        line_num++;
    }
    free(line);
    token_table_reset(assembler->token_table);

    // Second Pass - Code Generation
    int ram_address = 16;
    while (parser_has_more_commands(assembler->parser)) {
        if (!advance(assembler->parser)) break;
        if (assembler->parser->instruction->type == L_INSTRUCTION) continue;
        if (assembler->parser->instruction->type == A_INSTRUCTION_SYMBOL) {
            const char *symbol = assembler->parser->instruction->symbol;
            if (!symbol_table_contains(assembler->symbol_table, symbol)) {
                if (!symbol_table_add(assembler->symbol_table, symbol, ram_address++)) {
                    GLOG(LOG_ERROR, assembler->config.source_filepath, line_num,
                        "Failed to add symbol '%s' to symbol table.", symbol);
                    return_status = 1;
                    goto end;
                }
            }
            assembler->parser->instruction->value =
                symbol_table_get_address(assembler->symbol_table, symbol);
            assembler->parser->instruction->type = A_INSTRUCTION_VALUE;
        }

        // Generate binary instruction
        char binary_instruction[17] = {0};
        generate_binary(assembler->parser->instruction, binary_instruction);

        // Write to the .hack output file
        fprintf(assembler->config.target_hack, "%s\n", binary_instruction);
    }

    end:
    if (assembler->config.token_output) {
        token_table_write_to_file(assembler->config.token_output, assembler->token_table);
    }
    return return_status;  // 0 on success, 1 on failure
}
