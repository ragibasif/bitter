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

// #include <assert.h> //assert
// #include <ctype.h> //size_t
// #include <limits.h>
// #include <stdarg.h>
#include <stdbool.h> //true, false, bool
#include <stddef.h>
#include <stdint.h>
#include <stdio.h> //Includes the standard I/O library for functions like `printf`.
#include <stdlib.h> //Includes the standard library for functions like `malloc`, `free`, and `realloc`.

// #include <string.h> //Includes the string manipulation library for functions
// like `memcpy`.

// #include <time.h> //time

/******************************************************************************/
/*                          DEBUGGING - DO NOT TOUCH                          */
/******************************************************************************/

#define F_MEMORY_DEBUG /* turns on the memory debugging system */
#define F_MEMORY_PRINT /* turns on the memory printing system */
#define F_EXIT_CRASH   /* turns on the crash on exit */
#include "dbg-macro/dbg.h"
#include "forge/forge.h"
#include "munit/munit.h"

////////////////////////////////////////////////////////////////////////////////

// #include "memory.h"

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount)                          \
    (type *)reallocate(pointer, sizeof(type) * (oldCount),                     \
                       sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount)                                    \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void *reallocate(void *pointer, size_t oldSize, size_t newSize);

// #include "chunk.h"

typedef enum {
    // WARN: return from current function  (delete later)
    OP_RETURN,
} OpCode;

typedef struct {
    uint8_t *array;
    int size;
    int capacity;
} Chunk;

void chunk_create(Chunk *chunk);
void chunk_write(Chunk *chunk, uint8_t byte);
void chunk_destroy(Chunk *chunk);

// #include "debug.h"

void disassemble_chunk(Chunk *chunk, const char *name);
int disassemble_instruction(Chunk *chunk, int offset);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __BITTER_H__
