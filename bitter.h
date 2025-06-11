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

/******************************************************************************/
/*                                  INCLUDES                                  */
/******************************************************************************/

#include <assert.h> //assert
#include <ctype.h>  //size_t
#include <limits.h> // INT_MAX
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
// #define F_MEMORY_PRINT /* turns on the memory printing system */
// #define F_EXIT_CRASH /* turns on the crash on exit */
#include "libs/dbg-macro/dbg.h"
#include "libs/forge/forge.h"
#include "libs/munit/munit.h"

////////////////////////////////////////////////////////////////////////////////

#define max(a, b)                                                              \
    ({                                                                         \
        __typeof__(a) _a = (a);                                                \
        __typeof__(b) _b = (b);                                                \
        _a > _b ? _a : _b;                                                     \
    })

struct vm {
    struct data *data;
    struct lexer *lexer;
    int data_pointer;
    int highest_data_pointer;
    int instruction_pointer;
};

struct data {
    unsigned char *buffer;
    int size;
    int capacity;
};

struct lexer {
    char *source;
    int size;
    struct token **token_buffer;
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
    int location;       // only for ()
    int match_location; // only for ()
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

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __BITTER_H__
