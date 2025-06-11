/* File: main.c
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 *
 * Bitter esoteric programming language interpreter in C.
 */

// Bitter commands:

// >
// increment data pointer and invert bit

// <
// decrement data pointer and invert bit

// (
// if [p] == 1, proceed to next command, otherwise advance to command after
// matching ')'

// )
// go back to matching (

// !
// interpreter command: dump memory from 0 to highest value of data pointer

// #
// interpreter command: pause program and dump memory

#include "bitter.h"

static void test0(void) {
    char *source = ">>>>>!";
    run(source);
}

// static void test1(void) {
//     char *source = "<<<<<!";
//     run(source);
// }

static void test2(void) {
    char *source = ">(<)!";
    run(source);
}

// static void test3(void) {
//     char *source = ">><(><)";
//     decode(source);
// }

// static void test4(void) {
//     char *source = "(>><)>><!";
//     decode(source);
// }

// static void test5(void) {
//     char *source = ">><((>><)>><)!";
//     decode(source);
// }

// static void test6(void) {
//     char *source = "<";
//     decode(source);
// }

// static void test7(void) {
//     char *source = ">>><>>><>>><>>><>>><!";
//     decode(source);
// }

// static void test8(void) {
//     char *source = ">>><>>><>>><!";
//     decode(source);
// }

// static void test9(void) {
//     char *source = "!";
//     decode(source);
// }

int main(int argc, char **argv) {

    // TODO: add repl
    // TODO: read code from file
    // TODO: Proper error handling
    // TODO: example uses

    test0();
    // test1();
    test2();
    run("(((((())))))");
    run("((())))))");
    // run("((((((");
    // test3();
    // test4();
    // test5();
    // test6();
    // test7();
    // test8();
    // test9();

    /* MEMORY DEBUGGING - DO NOT TOUCH */
    f_debug_memory_init(NULL, NULL, NULL);
    f_debug_mem_print(0);
    f_debug_mem_reset();

    return EXIT_SUCCESS;
}
