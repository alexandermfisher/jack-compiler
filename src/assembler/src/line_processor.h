//
// Created by Alexander Fisher on 09/03/2025.
//

#ifndef LINE_PROCESSOR_H
#define LINE_PROCESSOR_H

#include "token_table.h"

#define MAX_LABEL_LEN 256

typedef enum {
    PROCESS_SUCCESS,   // Successfully processed a valid line
    PROCESS_EMPTY,     // Empty or comment line (ignore)
    PROCESS_INVALID,   // Syntax error in the line
    PROCESS_ERROR      // Critical failure (e.g., malloc failure)
} ProcessStatus;


ProcessStatus process_line(const char *line, TokenTable *table);

const char *preprocess_line(const char *line);

ProcessStatus process_label(const char *line, TokenTable *table);

ProcessStatus process_a_instruction(const char *line, TokenTable *table);

#endif //LINE_PROCESSOR_H
