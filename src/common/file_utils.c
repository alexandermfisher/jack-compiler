//
// Created by Alexander Fisher on 03/03/2025.
//
#include "file_utils.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>


bool has_extension(const char *filepath, const char *expected_extension) {
    if (!filepath || !expected_extension) { return false; }

    // Find the last '/' in the filepath to get the filename
    const char *filename = strrchr(filepath, '/');
    if (filename) {
        filename++;
    } else {
        filename = filepath;
    }

    // Find the last '.' to get the file extension
    const char *ext = strrchr(filename, '.');
    if (!ext) { return false; }

    return strcmp(ext, expected_extension) == 0;
}


bool change_file_extension(char *filepath, const size_t max_len, const char *extension) {
    if (!filepath || !extension) return false;

    // Reject empty filename, ".", ".."
    if (filepath[0] == '\0' || strcmp(filepath, ".") == 0 || strcmp(filepath, "..") == 0) {
        return false;
    }

    // Copy filepath into a local buffer
    char temp[max_len];
    strncpy(temp, filepath, max_len);
    temp[max_len - 1] = '\0';  // Ensure null termination

    // Find last '/' to get the filename
    const char *slash = strrchr(temp, '/');
    const char *filename = (slash) ? slash + 1 : temp;

    // Find last '.' after the last '/'
    char *dot = strrchr(filename, '.');

    // If dot is NULL or at the start of the filename (hidden file), append extension
    if (!dot || dot == filename) {
        const size_t total_len = strlen(temp) + strlen(extension) + 1;
        if (total_len > max_len) return false;
        snprintf(filepath, max_len, "%s%s", temp, extension);
        return true;
    }

    // Ensure new filename fits in output buffer
    const size_t base_len = dot - temp;
    if (base_len + strlen(extension) + 1 > max_len) return false;

    snprintf(filepath, max_len, "%.*s%s", (int)base_len, temp, extension);
    return true;
}

bool is_valid_filename(const char *filename) {
    return filename && strlen(filename) > 0 && strlen(filename) < PATH_MAX;
}

