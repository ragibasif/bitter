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
 * Bitter esoteric programming language interpreter in C.
 * https://esolangs.org/wiki/Bitter
 *
 */

#ifndef __BITTER_H__
#define __BITTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "libs/AEC.h" // ANSI escape codes
#include <stdarg.h>   // variadic functions
#include <stdbool.h>  //true, false, bool
#include <stdio.h> //Includes the standard I/O library for functions like `printf`.
#include <stdlib.h> //Includes the standard library for functions like `malloc`, `free`, and `realloc`.
#include <string.h> //Includes the string manipulation library for functions like `memcpy`.
#include <time.h> //time

/******************************************************************************/
/*                          DEBUGGING - DO NOT TOUCH                          */
/******************************************************************************/

// #define F_MEMORY_DEBUG /* turns on the memory debugging system */

#ifdef F_MEMORY_DEBUG
#define F_MEMORY_PRINT /* turns on the memory printing system */
#define F_EXIT_CRASH   /* turns on the crash on exit */
#include "libs/dbg.h"
#include "libs/forge.h"
#endif

/******************************************************************************/

#define MAX(a, b)                                                              \
    ({                                                                         \
        __typeof__(a) _a = (a);                                                \
        __typeof__(b) _b = (b);                                                \
        _a > _b ? _a : _b;                                                     \
    })

#define FLOOR_DIV(x, y) ((x) / (y))
#define CEIL_DIV(x, y) FLOOR_DIV((x) + (y - 1), y)

#define ERROR(fmt, ...)                                                        \
    error_print(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

void error_print(const char *file, const unsigned line, const char *function,
                 const char *format, ...);

struct vm {
    struct data *data;
    struct lexer *lexer;
    int64_t data_pointer;
    int highest_data_pointer;
    int instruction_pointer;
};

struct data {
    uint8_t *buffer;
    int size;
    int capacity;
};

struct lexer {
    char *source;
    int size;
    struct token **buffer;
};

enum token_type {
    GREATER = 1,     // >
    LESS = 2,        // <
    OPEN_PAREN = 3,  // (
    CLOSE_PAREN = 4, // )
    BANG = 5,        // !
    HASH = 6,        // #
};

struct token {
    enum token_type type;
    char literal;
    size_t location;       // only for ()
    size_t match_location; // only for ()
};

void vm_create(void);
void vm_destroy(void);

struct data *data_create(struct data *data);
void data_destroy(struct data *data);

struct lexer *lexer_create(struct lexer *lexer);
void lexer_init(char *source);
void lexer_destroy(struct lexer *lexer);

struct token *token_create(struct token *token, enum token_type type,
                           int location);
void token_destroy(struct token *token);

void run(char *source);
void tokenize(struct lexer *lexer);
void execute(void);

void memory_dump(enum token_type type);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __BITTER_H__
