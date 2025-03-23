#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdio.h>

typedef struct {
    FILE *source_asm;
    const char *source_filepath;
    FILE *target_hack;
    const char *target_filepath;
    FILE *token_output;
} AssemblerConfig;

// Forward declaration of the opaque Assembler type
typedef struct Assembler Assembler;

/**
 * @brief Create an Assembler instance with the provided configuration.
 * @param config Pointer to assembler configuration (caller-owned).
 * @return Pointer to Assembler instance, NULL on failure.
 */
Assembler *assembler_create(const AssemblerConfig *config);

/**
 * @brief Runs the assembly process.
 * @param assembler Assembler instance.
 * @return 0 on success, non-zero on failure.
 */
int assembler_assemble(Assembler *assembler);

/**
 * @brief Free the assembler instance and any allocated resources.
 * @param assembler Assembler instance to free.
 */
void assembler_free(Assembler *assembler);

#endif // ASSEMBLER_H
