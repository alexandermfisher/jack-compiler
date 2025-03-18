//
// Created by alexanderfisher on 06/03/25.
//

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>


/**
 * @brief Parses command-line arguments for the hackasm assembler.
 *
 * This function processes the command-line arguments to determine the source
 * assembly file, optional output file, and optional flags such as token printing.
 *
 * Supported options:
 *   -o / --output <output_file>    Specify the output file name.
 *   -t / --tokens                  Enable printing of tokens during processing.
 *   --                             Stop option parsing; remaining arguments are treated as positional.
 *
 * At minimum, a source file must be specified. The function will exit with
 * EXIT_FAILURE if required arguments are missing, duplicated options are provided,
 * or unrecognized arguments are encountered.
 *
 * @param argc          The argument count.
 * @param argv          The argument vector (array of strings).
 * @param source_file   Pointer to a char* where the source file name will be stored.
 * @param target_file   Pointer to a char* where the target file name (if any) will be stored.
 * @param print_tokens  Pointer to a bool that will be set true if token printing is enabled.
 */
void parse_arguments(int argc, char *argv[], char **source_file, char **target_file, bool *print_tokens);

#endif //UTIL_H
