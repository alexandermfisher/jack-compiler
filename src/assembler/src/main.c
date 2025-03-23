/**
 * Created by Alexander Fisher on 03/03/2025.
 *
 *
 * @brief Main entry point for the Hack Assembler (`hackasm`).
 *
 * @details
 * The assembler translates Hack Assembly (`.asm`) files into Hack Machine Code (`.hack`).
 * It processes command-line arguments to specify the source file, an optional target file,
 * and an optional flag to print tokens during processing.
 *
 * **Usage:**
 *   hackasm source.asm                   // Reads source.asm, writes to source.hack
 *   hackasm source.asm -o target.hack    // Writes to target.hack, reads source.asm
 *   hackasm source.asm -t                // Prints tokens during processing
 *   hackasm -o output.hack -t source.asm // Prints tokens and writes to output.hack
 *
 * **Command-line arguments:**
 *   - `source.asm` (required): The Hack assembly source file.
 *   - `-o target` or `--output target` (optional): Specify the target output filename.
 *     If omitted, `.hack` is added to the source filename.
 *   - `-t` or `--tokens` (optional): Enable printing of tokens during processing.
 *   - `--`: Stop argument parsing; all following arguments are positional.
 *
 * **Behavior:**
 *   - Generates the output file from the source file with proper extensions.
 *   - Reports errors for invalid filenames, missing source files, or incorrect usage.
 *   - Optionally prints tokens if the `-t` or `--tokens` flag is provided.
 *
 * **Examples:**
 *   hackasm add.asm                          → Generates `add.hack`
 *   hackasm -o my_output.hack add.asm        → Generates `my_output.hack`
 *   hackasm loop.asm -o custom.bin -t        → Generates `custom.bin` and prints tokens
 *   hackasm -t -o result.hack program.asm    → Generates `result.hack` and prints tokens
 */

#include <assembler.h>
#include <file_utils.h>
#include <logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/errno.h>

#define EXT_ASM ".asm"
#define EXT_HACK ".hack"

void parse_arguments(int argc, char *argv[], char **source_file, char **target_file, bool *print_tokens);

int main(const int argc, char *argv[]) {

    // Parse arguments
    char *source_file = NULL;
    char *target_file = NULL;
    bool print_tokens = false;
    parse_arguments(argc, argv, &source_file, &target_file, &print_tokens);

    // Validate source file extension
    if (!has_extension(source_file, EXT_ASM)) {
        fprintf(stderr, "Error: Source file must have '.asm' extension.\n");
        return EXIT_FAILURE;
    }

    // Check if source file exists
    if (access(source_file, F_OK) != 0) {
        fprintf(stderr, "Error: Source file '%s' does not exist.\n", source_file);
        return EXIT_FAILURE;
    }

    // Validate output filename (if provided)
    if (target_file) {
        if (!is_valid_filepath(target_file)) {
            fprintf(stderr, "Error: Invalid output filename.\n");
            return EXIT_FAILURE;
        }
    } else {
        // Generate default target filename (name.asm -> name.hack)
        static char default_target[PATH_MAX];
        const char *slash = strrchr(source_file, '/');
        const char *filename = (slash) ? slash + 1 : source_file;
        strncpy(default_target, filename, PATH_MAX);
        if (!change_file_extension(default_target, PATH_MAX, EXT_HACK)) {
            fprintf(stderr, "Error: Unable to generate target filename from.\n");
            return EXIT_FAILURE;
        }
        target_file = default_target;
    }

    // Prevent overwriting source file
    if (strcmp(source_file, target_file) == 0) {
        fprintf(stderr, "Error: Output file cannot be the same as source file.\n");
        return EXIT_FAILURE;
    }

    // Open source file for reading
    FILE *source_file_ptr = fopen(source_file, "r");
    if (!source_file_ptr) {
        fprintf(stderr, "Failed to open source file '%s': %s", source_file, strerror(errno));
        return EXIT_FAILURE;
    }

    // Open target file for writing (creates a new file if it doesn't exist)
    FILE *target_file_ptr = fopen(target_file, "w");
    if (!target_file_ptr) {
        fprintf(stderr, "Failed to open target file '%s': %s", target_file, strerror(errno));
        fclose(source_file_ptr);
        return EXIT_FAILURE;
    }

    // Open token.lex file output if required
    FILE *token_output_ptr = NULL;
    if (print_tokens) {
        const char *token_filename = "tokens.lex";
        token_output_ptr = fopen(token_filename, "w");
        if (!token_output_ptr) {
            fprintf(stderr, "Failed to open token output file '%s': %s", token_filename, strerror(errno));
            fclose(source_file_ptr);
            fclose(target_file_ptr);
            return EXIT_FAILURE;
        }
    }

    // Prepare assembler config
    const AssemblerConfig config = {
        .source_asm = source_file_ptr,
        .target_hack = target_file_ptr,
        .source_filepath = source_file,
        .target_filepath = target_file,
        .token_output = token_output_ptr,
    };

    // Create assembler
    Assembler *assembler = assembler_create(&config);
    if (!assembler) {
        GLOG(LOG_ERROR, "Failed to initialise assembler.");
        fclose(source_file_ptr);
        fclose(target_file_ptr);
        return EXIT_FAILURE;
    }

    // Initialize global logger
    Logger *logger = logger_create(NULL, LOG_INFO, true);
    if (!logger) {
        fprintf(stderr, "Failed to initialize logger\n");
        return EXIT_FAILURE;
    }
    logger_set_global(logger);

    GLOG(LOG_INFO, "Hack Assembler started");

    // Run assembler
    const int status = assembler_assemble(assembler);

    // Dump log contents to terminal if error occurred
    if (status != 0) logger_dump(logger, stderr);

    // Clean up
    logger_free(logger);
    assembler_free(assembler);
    fclose(token_output_ptr);
    fclose(source_file_ptr);
    fclose(target_file_ptr);

    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

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
void parse_arguments(const int argc, char *argv[], char **source_file, char **target_file, bool *print_tokens) {
    int i = 1;
    bool end_of_options = false;

    while (i < argc) {
        if (!end_of_options && strcmp(argv[i], "--") == 0) {
            // End of options, treat all following arguments as positional
            end_of_options = true;
            i++;
            continue;
        }

        if (!end_of_options && (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)) {
            // Optional Argument: -o <target_file>
            if (i + 1 < argc) {
                if (*target_file != NULL) {
                    fprintf(stderr, "Error: Multiple -o options are not allowed.\n");
                    fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                *target_file = argv[++i];
            } else {
                fprintf(stderr, "Error: -o requires a target file.\n");
                fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (!end_of_options && (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tokens") == 0)) {
            // Toggle printing of tokens
            *print_tokens = true;
        } else if (*source_file == NULL) {
            // Positional argument: <source_file>
            *source_file = argv[i];
        } else {
            fprintf(stderr, "Error: Unrecognized argument '%s'.\n", argv[i]);
            fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        i++;
    }

    if (*source_file == NULL) {
        fprintf(stderr, "Error: Source file is required.\n");
        fprintf(stderr, "Usage: %s [-o output.hack] [-t|--tokens] source.asm\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
