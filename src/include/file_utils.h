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
 * @param filename The original filename.
 * @param new_extension The new file extension (e.g., ".hack").
 * @param output A buffer to store the new filename.
 * @param output_size The size of the output buffer.
 * @return true if successful, false if the filename is too long.
 */
bool change_extension(const char *filename, const char *new_extension, char *output, size_t output_size);

#endif // FILE_UTILS_H

