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
 * Bitter esoteric programming language interpreter in C.
 */

#include "bitter.h"

// TODO
struct data *data_create(struct data *tape) {
    size_t i;
    tape = malloc(sizeof(*tape));
    tape->buffer = malloc(MAX_BUFFER * sizeof(*tape->buffer));
    tape->size = MAX_BUFFER;
    tape->capacity = MAX_BUFFER;
    tape->data_pointer = 0;
    tape->data_pointer_max = 0;

    for (i = 0; i < tape->capacity; i++) {
        tape->buffer[i] = 0;
    }
    return tape;
}

void data_destroy(struct data *tape) {
    tape->size = 0;
    tape->capacity = 0;
    tape->data_pointer = 0;
    tape->data_pointer_max = 0;
    free(tape->buffer);
    tape->buffer = NULL;
    free(tape);
    tape = NULL;
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

void find_close_paren(struct lexer *lexer, size_t instruction_pointer) {
    while (lexer->token_buffer[instruction_pointer].type != CLOSE_PAREN &&
           instruction_pointer < MAX_BUFFER - 1) {
        instruction_pointer++;
    }
}

void find_open_paren(struct lexer *lexer, size_t instruction_pointer) {
    while (lexer->token_buffer[instruction_pointer].type != OPEN_PAREN &&
           instruction_pointer > 0) {
        instruction_pointer--;
    }
}

void execute(struct lexer *lexer) {
    size_t instruction_pointer;
    size_t open_paren_pointer;
    size_t close_paren_pointer;
    instruction_pointer = 0;
    struct data *tape;
    tape = data_create(tape);
    for (instruction_pointer = 0; instruction_pointer < MAX_BUFFER;
         instruction_pointer++) {
        switch (lexer->token_buffer[instruction_pointer].type) {
        case (GREATER): {
            tape->data_pointer++;
            tape->data_pointer_max =
                max(tape->data_pointer_max, tape->data_pointer);
            tape->buffer[tape->data_pointer] =
                invert_bit[tape->buffer[tape->data_pointer]];
            break;
        }
        case (LESS): {
            tape->data_pointer_max =
                max(tape->data_pointer_max, tape->data_pointer);
            tape->data_pointer--;
            tape->buffer[tape->data_pointer] =
                invert_bit[tape->buffer[tape->data_pointer]];
            break;
        }
        case (OPEN_PAREN): {
            open_paren_pointer = instruction_pointer;
            if (!tape->buffer[tape->data_pointer]) {
                find_close_paren(lexer, instruction_pointer);
            }
            break;
        }
        case (CLOSE_PAREN): {
            find_open_paren(lexer, instruction_pointer);
            break;
        }
        case (BANG): {
            dump_used_memory(tape);
            break;
        }
        case (HASH): {
            dump_full_memory(tape);
            break;
        }
        default: {
            break;
        }
        }
    }
    data_destroy(tape);
}

struct lexer *lexer_create(struct lexer *lexer, char *source) {
    lexer->size = strlen(source);
    lexer = malloc(sizeof(*lexer));
    lexer->source = malloc(1 + lexer->size);
    memcpy(lexer->source, source, lexer->size);
    lexer->source[lexer->size] = '\0';
    lexer->token_buffer = malloc(lexer->size * sizeof(*lexer->token_buffer));
    return lexer;
}

void lexer_destroy(struct lexer *lexer) {
    free(lexer->source);
    lexer->source = NULL;
    free(lexer->token_buffer);
    lexer->token_buffer = NULL;
    free(lexer);
    lexer = NULL;
}

void decode(char *source) {
    size_t src_len;
    struct lexer *lexer;
    lexer = lexer_create(lexer, source);
    src_len = strlen(lexer->source);
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

    lexer_destroy(lexer);
}

void dump_full_memory(struct data *tape) {
    size_t i;
    for (i = 0; i < MAX_BUFFER; i++) {
        printf("%u ", tape->buffer[i]);
    }
    putchar('\n');
}

void dump_used_memory(struct data *tape) {
    size_t i;
    for (i = 0; i < tape->data_pointer_max; i++) {
        printf("%u ", tape->buffer[i]);
    }
    putchar('\n');
}
