//
// Created by Alexander Fisher on 24/03/2025.
//
#include "file_list.h"
#include "file_utils.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>


bool file_list_add(FileList *list, const char *path, const char *extension) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return false;

    if (S_ISREG(path_stat.st_mode)) {
        // It's a file
        if (has_extension(path, extension)) {
            // TODO: Add FileEntry to list
            printf("Found file: %s\n", path);
            return true;
        }
    } else if (S_ISDIR(path_stat.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) return false;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            char full_path[MAX_PATH_LEN];
            snprintf(full_path, MAX_PATH_LEN, "%s/%s", path, entry->d_name);

            struct stat file_stat;
            if (stat(full_path, &file_stat) != 0) continue; // Skip unreadable entries
            if (!S_ISREG(file_stat.st_mode)) continue;      // Only process regular files

            if (has_extension(entry->d_name, extension)) {
                // TODO: Add FileEntry to list
                printf("Found file in dir: %s\n", full_path);
            }
        }
        closedir(dir);
    }
    return true;
}
