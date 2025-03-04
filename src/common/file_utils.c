//
// Created by Alexander Fisher on 03/03/2025.
//

#include "file_utils.h"
#include <string.h>
#include <stdbool.h>

bool has_extension(const char *filename, const char *expected_extension) {
    if (!filename || !expected_extension) {
        return false;
    }

    const char *ext = strrchr(filename, '.');
    return ext && strcmp(ext, expected_extension) == 0;
}


bool change_extension(const char *filename, const char *new_extension, char *output, size_t output_size) {
    if (!filename || !new_extension || !output) return false;

    // Find last '.' in filename
    const char *dot = strrchr(filename, '.');
    size_t base_length = (dot) ? (size_t)(dot - filename) : strlen(filename);

    // Ensure new filename fits in output buffer
    if (base_length + strlen(new_extension) + 1 > output_size) return false;

    // Copy base name and append new extension
    strncpy(output, filename, base_length);
    output[base_length] = '\0';
    strcat(output, new_extension);

    return true;
}
