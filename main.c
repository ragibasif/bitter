/* File: main.c
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#include "bitter.h"

void test_case1(void);
void test_case2(void);
void test_case3(void);
void test_case4(void);
void test_case5(void);
void test_case6(void);
void test_case7(void);
void test_case8(void);
void test_case9(void);

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    test_case1();
    test_case2();
    test_case3();
    // test_case4();
    // test_case5();
    // test_case6();
    // test_case7();
    // test_case8();
    // test_case9();

    /* MEMORY DEBUGGING - DO NOT TOUCH */
    f_debug_memory_init(NULL, NULL, NULL);
    f_debug_mem_print(0);
    f_debug_mem_reset();

    return 0;
}

char *str;
char *expected;
size_t len;

#define RUN                                                                    \
    do {                                                                       \
        len = strlen(str);                                                     \
        memory_create();                                                       \
        get_input(str, len);                                                   \
        memory_destroy();                                                      \
    } while (0)

void test_case1(void) {
    str = ">>!#";
    expected = "[0,1,1]";
    dbg(str);
    dbg(expected);
    RUN;
}

void test_case2(void) {
    str = ">><<!#";
    expected = "[1,0,1]";
    dbg(str);
    dbg(expected);
    RUN;
}

void test_case3(void) {
    str = ">#<!";
    expected = "[0,1],[1,1]";
    dbg(str);
    dbg(expected);
    RUN;
}

void test_case4(void) {}

void test_case5(void) {}

void test_case6(void) {}

void test_case7(void) {}

void test_case8(void) {}

void test_case9(void) {}
