//
// Created by Alexander Fisher on 03/03/2025.
//
#include "file_utils.h"
#include <string.h>
#include <stdbool.h>


bool has_extension(const char *filepath, const char *expected_extension) {
    if (!filepath || !expected_extension) { return false; }

    // Find the last '/' in the filepath to get the filename
    const char *filename = strrchr(filepath, '/');
    if (filename) {
        filename++;
    } else {
        filename = filepath;
    }

    // Find the first '.' to get the file extension
    const char *ext = strchr(filename, '.');
    if (!ext) { return false; }

    return strcmp(ext, expected_extension) == 0;
}





bool change_extension(const char *filepath, const char *new_extension, char *output, const size_t output_size) {
    if (!filepath || !new_extension || !output) return false;

    // Find the last '/' in the filepath to get the filename
    const char *filename = strrchr(filepath, '/');
    if (filename) {
        filename++;
    } else {
        filename = filepath;
    }



    // Find last '.' in filename
    const char *dot = strchr(filename, '.');
    const size_t base_length = (dot) ? (size_t)(dot - filepath) : strlen(filepath);

    // Ensure new filename fits in output buffer
    if (base_length + strlen(new_extension) + 1 > output_size) return false;

    // Copy base name and append new extension
    strncpy(output, filename, base_length);
    output[base_length] = '\0';
    strcat(output, new_extension);

    return true;
}
