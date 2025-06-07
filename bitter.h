/*
 * File: bitter.h
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 *
 * https://esolangs.org/wiki/Bitter
 *
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

#ifndef __BITTER_H__
#define __BITTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/******************************************************************************/
/*                                  INCLUDES                                  */
/******************************************************************************/

#include <assert.h> //assert
#include <ctype.h>  //size_t
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h> //true, false, bool
#include <stddef.h>
#include <stdint.h>
#include <stdio.h> //Includes the standard I/O library for functions like `printf`.
#include <stdlib.h> //Includes the standard library for functions like `malloc`, `free`, and `realloc`.
#include <string.h> //Includes the string manipulation library for functions like `memcpy`.
#include <time.h> //time

/******************************************************************************/
/*                          DEBUGGING - DO NOT TOUCH                          */
/******************************************************************************/

#define F_MEMORY_DEBUG /* turns on the memory debugging system */
#define F_MEMORY_PRINT /* turns on the memory printing system */
#define F_EXIT_CRASH   /* turns on the crash on exit */
#include "libs/dbg-macro/dbg.h"
#include "libs/forge/forge.h"
#include "libs/munit/munit.h"

////////////////////////////////////////////////////////////////////////////////

#define MAX_BUFFER 10

enum token_type {
    GREATER,     // >
    LESS,        // <
    OPEN_PAREN,  // (
    CLOSE_PAREN, // )
    BANG,        // !
    HASH,        // #
};

struct token {
    enum token_type type;
    char literal;
};

void token_create(struct token *token, enum token_type type);

struct lexer {
    char *source;
    struct token token_buffer[MAX_BUFFER];
};

void dump_used_memory(void);

void dump_full_memory(void);

void execute(struct lexer *lexer);

void run(char *source);

void find_close_paren(size_t instruction_pointer);
void find_open_paren(size_t instruction_pointer);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __BITTER_H__
