//
// Created by alexanderfisher on 31/05/25.
//
#include "file_list.h"
#include <assert.h>
#include <file_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>     // for unlink, rmdir

void test_file_list_add(void);

int main(void) {
    test_file_list_add();
    printf("        ✅ test_file_list_add passed!\n");
    return 0;
}

void make_file(const char *path) {
    FILE *f = fopen(path, "w");
    if (f) fclose(f);
}

void test_file_list_add(void) {
    // Setup: create test dir structure
    mkdir("testdir", 0755);
    mkdir("testdir/subdir", 0755);

    make_file("testdir/a.vm");           // should match
    make_file("testdir/b.txt");          // should not match
    make_file("testdir/subdir/c.vm");    // should match
    make_file("testdir/subdir/d.jack");  // should not match

    FileList *list = file_list_create();
    assert(list != NULL);

    bool ok = file_list_add(list, "testdir", EXT_VM);
    assert(ok == true);
    assert(list->count == 2);

    // // Optional: print paths for debugging
    // for (size_t i = 0; i < list->count; ++i) {
    //     printf("Matched: %s\n", list->files[i].full_path);
    //     assert(has_extension(list->files[i].full_path, EXT_VM));
    // }

    // Clean up memory
    file_list_free(list);

    // Clean up test files and directories
    unlink("testdir/a.vm");
    unlink("testdir/b.txt");
    unlink("testdir/subdir/c.vm");
    unlink("testdir/subdir/d.jack");
    rmdir("testdir/subdir");
    rmdir("testdir");
}
