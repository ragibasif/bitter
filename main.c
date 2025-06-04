/* File: main.c
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

// Bitter commands:

// >
// increment data pointer and invert bit
//
// <
// decrement data pointer and invert bit
//
// (
// if [p] == 1, proceed to next command, otherwise advance to command after
// matching ')'
//
// )
// go back to matching (
//
// !
// interpreter command: dump memory from 0 to highest value of data pointer
//
// #
// interpreter command: pause program and dump memory

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

    Chunk chunk;
    chunk_create(&chunk);
    chunk_write(&chunk, OP_RETURN);
    disassemble_chunk(&chunk, "test chunk");
    chunk_destroy(&chunk);

    // test_case1();
    // test_case2();
    // test_case3();
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

    return EXIT_SUCCESS;
}

void test_case1(void) {}

void test_case2(void) {}

void test_case3(void) {}

void test_case4(void) {}

void test_case5(void) {}

void test_case6(void) {}

void test_case7(void) {}

void test_case8(void) {}

void test_case9(void) {}
