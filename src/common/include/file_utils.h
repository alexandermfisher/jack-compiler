//
// Created by Alexander Fisher on 03/03/2025.
//
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdbool.h>


/**
 * @brief Checks if a filename has a specific extension.
 *
 * @param filepath The name of the file.
 * @param expected_extension The expected file extension (e.g., ".asm").
 * @return true if the filename has the expected extension, false otherwise.
 */
bool has_extension(const char *filepath, const char *expected_extension);


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
bool change_file_extension(char *filepath, size_t max_len,  const char *extension);


/**
 * @brief Checks if a given path represents a valid file, not a directory.
 *
 * This function ensures that the input is a valid file path:
 *   - Allows absolute ("/file.txt") and relative ("file.txt") paths.
 *   - Rejects the root directory ("/") and standalone "." or "..".
 *   - Ensures the path does not end with '/' (indicating a directory).
 *
 * @param filepath The file path to validate.
 * @return true if it is a valid file path, false otherwise.
 */
bool is_valid_filepath(const char *filepath);






#endif // FILE_UTILS_H

