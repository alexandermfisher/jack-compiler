//
// Created by Alexander Fisher on 03/03/2025.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Checks if a filename has a specific extension.
 *
 * @param filename The name of the file.
 * @param expected_extension The expected file extension (e.g., ".asm").
 * @return true if the filename has the expected extension, false otherwise.
 */
bool has_extension(const char *filename, const char *expected_extension);


/**
 * @brief Changes the file extension of a given filename.
 *
 * This function takes a filename and replaces its extension with a new one.
 * If the filename has an existing extension (i.e., contains a '.'), the new extension replaces it.
 * If there is no extension, the new extension is simply appended to the filename.
 *
 * @param filename      The original filename (must be a null-terminated string).
 * @param new_extension The new extension to append or replace the old one (should start with '.').
 * @param output        The buffer to store the new filename.
 * @param output_size   The size of the output buffer.
 * @return true on success, false if any argument is NULL or the output buffer is too small.
 *
 * @note The function does not check if new_extension starts with a '.'.
 *       The caller should ensure the correct format.
 */
bool change_extension(const char *filename, const char *new_extension, char *output, size_t output_size);

#endif // FILE_UTILS_H

