/*
 * File: bitter.c
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 *
 * implementation
 *
 */

#include "bitter.h"

#define MAX_BUFFER 10

static void dbg_token(struct token *token) {
    dbg(token);
    dbg(token->type);
    dbg(token->literal);
}

static void dbg_lexer(struct lexer *lexer) {
    dbg(&lexer);
    dbg(lexer);
    dbg(&lexer->source);
    dbg(lexer->source);
    dbg(&lexer->token_buffer);
    dbg(lexer->token_buffer);
    size_t i;
    for (i = 0; i < MAX_BUFFER; i++) {
        // *(((int *)array->buffer) + i) = i + 100;
        dbg(&*(((struct token *)lexer->token_buffer) + i));
        dbg((((struct token *)lexer->token_buffer) + i)->literal);
        dbg(*&(((struct token *)lexer->token_buffer) + i)->type);
    }
}

////////////////////////////////////////////////////////////////////////////////

// FIX: turn this into a dynamic array
static char data_buffer[MAX_BUFFER];
static char invert_bit[2] = {1, 0};

void token_create(struct token *token, enum token_type type) {
    switch (type) {
    case (GREATER): {
        token->type = GREATER;
        token->literal = '>';
        break;
    }
    case (LESS): {
        token->type = LESS;
        token->literal = '<';
        break;
    }
    case (OPEN_PAREN): {
        token->type = OPEN_PAREN;
        token->literal = '(';
        break;
    }
    case (CLOSE_PAREN): {
        token->type = CLOSE_PAREN;
        token->literal = ')';
        break;
    }
    case (BANG): {
        token->type = BANG;
        token->literal = '!';
        break;
    }
    case (HASH): {
        token->type = HASH;
        token->literal = '#';
        break;
    }
    default:
        fprintf(stderr, "Did not recognize token!\n");
        exit(1);
    }
    // dbg_token(token);
}

void find_close_paren(size_t instruction_pointer) {}
void find_open_paren(size_t instruction_pointer) {}

void execute(struct lexer *lexer) {
    size_t instruction_pointer;
    size_t data_pointer;
    size_t open_paren_pointer;
    size_t close_paren_pointer;
    instruction_pointer = 0;
    data_pointer = 0;
    open_paren_pointer = 0;
    close_paren_pointer = 0;
    for (instruction_pointer = 0; instruction_pointer < MAX_BUFFER;
         instruction_pointer++) {
        switch (lexer->token_buffer[instruction_pointer].type) {
        case (GREATER): {
            data_pointer++;
            data_buffer[data_pointer] = invert_bit[data_buffer[data_pointer]];
            break;
        }
        case (LESS): {
            data_pointer--;
            data_buffer[data_pointer] = invert_bit[data_buffer[data_pointer]];
            break;
        }
        case (OPEN_PAREN): {
            open_paren_pointer = instruction_pointer;
            if (!data_buffer[data_pointer]) {
                instruction_pointer = close_paren_pointer + 1;
            }
            break;
        }
        case (CLOSE_PAREN): {
            close_paren_pointer = instruction_pointer;
            instruction_pointer = open_paren_pointer;
            break;
        }
        case (BANG): {
            dump_used_memory();
            break;
        }
        case (HASH): {
            dump_full_memory();
            break;
        }
        default: {
            break;
        }
        }
    }
}

void lexer_create(struct lexer *lexer, char *source) {}
void lexer_destroy(struct lexer *lexer) {}

void run(char *source) {
    struct lexer *lexer;
    size_t src_len = strlen(source);
    lexer = malloc(sizeof(*lexer));
    lexer->source = malloc(1 + src_len);
    memcpy(lexer->source, source, src_len);
    lexer->source[src_len] = '\0';

    char c;
    size_t i;
    i = 0;

    while (i < src_len) {
        c = lexer->source[i];
        struct token token;
        switch (c) {
        case ('>'): {
            token_create(&token, GREATER);
            break;
        }
        case ('<'): {
            token_create(&token, LESS);
            break;
        }
        case ('('): {
            token_create(&token, OPEN_PAREN);
            break;
        }
        case (')'): {
            token_create(&token, CLOSE_PAREN);
            break;
        }
        case ('!'): {
            token_create(&token, BANG);
            break;
        }
        case ('#'): {
            token_create(&token, HASH);
            break;
        }
        default: {
            break;
        }
        }
        lexer->token_buffer[i++] = token;
    }

    execute(lexer);

    // dbg_lexer(lexer);
    free(lexer->source);
    lexer->source = NULL;
    free(lexer);
    lexer = NULL;
}

void dump_full_memory() {
    size_t i;
    for (i = 0; i < MAX_BUFFER; i++) {
        printf("%d ", data_buffer[i]);
    }
    putchar('\n');
}

// FIX: track the highest that the data pointer goes and dump memory upto there
void dump_used_memory() {
    size_t i;
    for (i = 0; i < MAX_BUFFER; i++) {
        printf("%d ", data_buffer[i]);
    }
    putchar('\n');
}
