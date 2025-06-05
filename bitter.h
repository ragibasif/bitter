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

// #include "value.h"

// data

// can either be 0 or 1
typedef uint8_t Bit;

// aka Value
typedef struct {
    Bit *bits;
    int size;
    int capacity;
} BitArray;

void bit_array_create(BitArray *array);
void bit_array_write(BitArray *array, Bit value);
void bit_array_destroy(BitArray *array);

// #include "chunk.h"

// input instructions

typedef enum {
    OP_RETURN,
    OP_INC_INVERT,    // > - move data ptr to the right and invert bit
    OP_DEC_INVERT,    // < - move data ptr to the left and invert bit
    OP_START_LOOP,    // ( - if [p] == 1 then next command, else matching ) + 1
    OP_END_LOOP,      // ) - back to matching (
    OP_USED_MEM_DUMP, // ! - dump mem from 0 to highest data ptr
    OP_FULL_MEM_DUMP, // # - pause program and dump full memory
} OpCode;

// aka Chunk
typedef struct {
    uint8_t *code;
    int size;
    int capacity;
} Commands;

void commands_create(Commands *command);
void commands_write(Commands *command, uint8_t byte);
void commands_destroy(Commands *command);

#define STACK_MAX 256

typedef struct {
    Commands *commands;
    uint8_t *ip;
    Bit stack[STACK_MAX];
    Bit *stack_top;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void vm_create();
void vm_destroy();
InterpretResult interpret(const char *source);
// void push(Bit bit);
// Bit pop();

// #include "compiler.h"
void compile(const char *source);

// #include "scanner.h"

void scanner_create(const char *source);

// #include "debug.h"

void disassemble_commands(Commands *commands, const char *name);
int disassemble_instruction(Commands *commands, int offset);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __BITTER_H__
