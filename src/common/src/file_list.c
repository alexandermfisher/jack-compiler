//
// Created by Alexander Fisher on 24/03/2025.
//
#include "file_list.h"
#include "file_utils.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// --- Helper functions ---

bool file_list_append(FileList *list, const char *full_path) {
    size_t capacity = list->count + 1;
    FileEntry *new_files = realloc(list->files, capacity * sizeof(FileEntry));
    if (!new_files) return false;
    list->files = new_files;

    char *path_copy = strdup(full_path);
    if (!path_copy) return false;

    const char *filename = strrchr(path_copy, '/');
    filename = filename ? filename + 1 : path_copy;
    char *dot = strrchr(filename, '.');
    size_t base_len = dot ? (size_t)(dot - filename) : strlen(filename);
    char *base_name = strndup(filename, base_len);
    if (!base_name) {
        free(path_copy);
        return false;
    }

    char *last_slash = strrchr(full_path, '/');
    size_t source_len = last_slash ? (size_t)(last_slash - full_path + 1) : 0;
    char *source = strndup(full_path, source_len);
    if (!source) {
        free(path_copy);
        free(base_name);
        return false;
    }

    list->files[list->count].full_path = path_copy;
    list->files[list->count].base_name = base_name;
    list->files[list->count].source = source;
    list->count++;

    return true;
}

static int file_entry_compare(const void *a, const void *b) {
    const FileEntry *fa = a;
    const FileEntry *fb = b;
    return strcmp(fa->full_path, fb->full_path);
}

// --- FileList API implementation ---

FileList *file_list_create(void) {
    FileList *list = malloc(sizeof(FileList));
    if (!list) return NULL;

    list->files = NULL;
    list->count = 0;
    list->index = 0;
    return list;
}

bool file_list_add(FileList *list, const char *path, const char *extension) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return false;

    if (S_ISREG(path_stat.st_mode)) {
        if (has_extension(path, extension)) {
            if (!file_list_append(list, path)) return false;
            printf("Added file: %s\n", path);
        }
    } else if (S_ISDIR(path_stat.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) return false;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char full_path[MAX_PATH_LEN];
            snprintf(full_path, MAX_PATH_LEN, "%s/%s", path, entry->d_name);

            if (stat(full_path, &path_stat) != 0) continue;

            if (S_ISDIR(path_stat.st_mode)) {
                if (!file_list_add(list, full_path, extension)) {
                    closedir(dir);
                    return false;
                }
            } else if (S_ISREG(path_stat.st_mode)) {
                if (has_extension(entry->d_name, extension)) {
                    if (!file_list_append(list, full_path)) {
                        closedir(dir);
                        return false;
                    }
                    printf("Added file: %s\n", full_path);
                }
            }
        }
        closedir(dir);
    }

    return true;
}

void file_list_sort(FileList *list) {
    if (list && list->files && list->count > 1) {
        qsort(list->files, list->count, sizeof(FileEntry), file_entry_compare);
    }
}

FILE *file_list_open_next(FileList *list) {
    if (!list || list->index >= list->count) return NULL;

    const char *path = list->files[list->index].full_path;
    list->index++;

    return fopen(path, "r");
}

const char *file_list_current_basename(FileList *list) {
    if (!list || list->index == 0 || list->index > list->count) return NULL;
    return list->files[list->index - 1].base_name;
}

const char *file_list_current_source(FileList *list) {
    if (!list || list->index == 0 || list->index > list->count) return NULL;
    return list->files[list->index - 1].source;
}

void file_list_reset(FileList *list) {
    if (list) list->index = 0;
}

void file_list_free(FileList *list) {
    if (!list) return;

    for (size_t i = 0; i < list->count; ++i) {
        free(list->files[i].full_path);
        free(list->files[i].base_name);
        free(list->files[i].source);
    }

    free(list->files);
    free(list);
}

bool file_list_has_file(const FileList *list, const char *filename) {
    if (!list || !filename) return false;

    for (size_t i = 0; i < list->count; ++i) {
        const char *file = strrchr(list->files[i].full_path, '/');
        file = file ? file + 1 : list->files[i].full_path;

        if (strcmp(file, filename) == 0) {
            return true;
        }
    }
    return false;
}
