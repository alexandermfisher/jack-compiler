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
 * @brief Changes the file extension of a given filepath.
 *
 * This function modifies the input `filepath` in place, replacing its current
 * extension with the specified `extension`. If the file has no extension, the
 * new extension is appended. The function ensures the resulting filename does
 * not exceed `max_len` characters.
 *
 * @param filepath The file path to modify. Must be a valid, non-null, mutable string.
 * @param max_len The maximum allowed length for the modified filepath, including the null terminator.
 * @param extension The new extension to apply, including the leading dot (e.g., ".txt").
 * @return true if the extension was successfully changed or appended, false on failure.
 *
 * @note The function rejects invalid filenames such as empty strings, ".", and "..".
 * @note If `filepath` is too long after modification, the function fails and returns false.
 * @note The `filepath` buffer must be large enough to accommodate the modified string.
 */
bool change_file_extension(char *filepath, const size_t max_len,  const char *extension);



bool is_valid_filename(const char *filename);


#endif // FILE_UTILS_H

