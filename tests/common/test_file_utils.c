//
// Created by Alexander Fisher on 03/03/2025.
//
#include "../../src/common/file_utils.h"
#include <assert.h>
#include <stdio.h>

void test_has_extension() {
    assert(has_extension("test.asm", ".asm") == true);
    assert(has_extension("test.hack", ".hack") == true);
    assert(has_extension("test", ".asm") == false);
    assert(has_extension(NULL, ".asm") == false);
    assert(has_extension("test.asm", NULL) == false);
}

int main() {
    test_has_extension();
    return 0;
}