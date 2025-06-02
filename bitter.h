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

#define MAX_BUFFER 10

#define MAX(T, a, b)                                                           \
    ({                                                                         \
        T temp_a = (a);                                                        \
        T temp_b = (b);                                                        \
        temp_a > temp_b ? temp_a : temp_b;                                     \
    }) // any type

#define ZERO 0
#define ONE 1

#define TOKEN_INC '>'
#define TOKEN_DEC '<'
#define TOKEN_LOOP_START '('
#define TOKEN_LOOP_END ')'
#define TOKEN_DUMP_TO_DATA_PTR '!'
#define TOKEN_PAUSE_AND_DUMP_MEM_FULL '#'

typedef struct Tape_t {
    uint8_t *buffer;
    size_t size;
    size_t pointer;
    size_t highest_pointer;
} Tape;

void memory_create(void);
void memory_destroy(void);
void memory_dump(size_t size);
uint8_t invert_bit(uint8_t bit);
void lexer(char *str, size_t size);
void execute(void);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __BITTER_H__
