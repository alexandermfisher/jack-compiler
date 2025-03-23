#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

// Supported file types (extensions)
#define EXT_VM   ".vm"
#define EXT_JACK ".jack"

// Maximum length of file paths (adjust if needed)
#define MAX_PATH_LEN 4096

// Represents a single file entry with its path, base name, and source directory
typedef struct {
    char *full_path;   // Full file path, e.g., "./input/Foo.vm"
    char *base_name;   // Base name without extension, e.g., "Foo"
    char *source;      // Source directory the file came from, e.g., "./input/"
} FileEntry;

// Holds a collection of files
typedef struct {
    FileEntry *files;  // Dynamic array of FileEntry
    size_t count;      // Number of files
    size_t index;      // Iterator index (for file_list_open_next)
} FileList;

/**
 * Creates an empty FileList (caller must free with file_list_free).
 *
 * @return Pointer to FileList or NULL on failure
 */
FileList *file_list_new(void);

/**
 * Adds matching files from a path (file or directory) to the FileList.
 * If 'path' is a file, adds it if it matches the given extension.
 * If 'path' is a directory, scans and adds matching files.
 *
 * @param list Pointer to an existing FileList
 * @param path File or directory path
 * @param extension Extension to match (e.g., EXT_VM or EXT_JACK)
 * @return true on success, false on error
 */
bool file_list_add(FileList *list, const char *path, const char *extension);

/**
 * Sorts the FileList alphabetically by full_path for deterministic output.
 */
void file_list_sort(FileList *list);

/**
 * Opens the next file in the list for reading.
 * Returns NULL when all files have been processed.
 *
 * @param list Pointer to FileList
 * @return FILE* handle (caller must fclose), or NULL if done
 */
FILE *file_list_open_next(FileList *list);

/**
 * Gets the current file's base name (no extension, no path).
 *
 * @param list Pointer to FileList
 * @return const char* to base name, or NULL if out of bounds
 */
const char *file_list_current_basename(FileList *list);

/**
 * Gets the source directory of the current file.
 *
 * @param list Pointer to FileList
 * @return const char* to source directory
 */
const char *file_list_current_source(FileList *list);

/**
 * Resets the iteration index to the beginning of the list.
 * Use before re-iterating with file_list_open_next.
 */
void file_list_reset(FileList *list);

/**
 * Frees all memory allocated for the FileList and its contents.
 */
void file_list_free(FileList *list);

#endif // FILE_LIST_H
