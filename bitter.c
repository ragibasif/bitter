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

#define DEFAULT_BUFFER 1024

struct vm vm;

void vm_create() {
    vm.data = data_create(vm.data);
    vm.lexer = lexer_create(vm.lexer);
    vm.data_pointer = 0;
    vm.instruction_pointer = 0;
}

void vm_destroy() {
    data_destroy(vm.data);
    lexer_destroy(vm.lexer);
}

struct data *data_create(struct data *data) {
    size_t i;
    data = malloc(sizeof(*data));
    data->buffer = malloc(DEFAULT_BUFFER * sizeof(*data->buffer));
    data->size = DEFAULT_BUFFER;
    data->capacity = DEFAULT_BUFFER;

    for (i = 0; i < data->capacity; i++) {
        data->buffer[i] = 0;
    }
    return data;
}

void data_destroy(struct data *data) {
    data->size = 0;
    data->capacity = 0;
    free(data->buffer);
    data->buffer = NULL;
    free(data);
    data = NULL;
}

struct lexer *lexer_create(struct lexer *lexer) {
    lexer = malloc(sizeof(*lexer));

    lexer->src_size = DEFAULT_BUFFER;
    lexer->src_capacity = DEFAULT_BUFFER;
    lexer->source = malloc(lexer->src_capacity * sizeof(*lexer->source));

    lexer->buf_size = DEFAULT_BUFFER;
    lexer->buf_capacity = DEFAULT_BUFFER;
    lexer->token_buffer =
        malloc(lexer->buf_capacity * sizeof(*lexer->token_buffer));

    return lexer;
}

void lexer_destroy(struct lexer *lexer) {
    size_t i;

    free(lexer->source);
    lexer->source = NULL;
    lexer->src_size = 0;
    lexer->src_capacity = 0;

    for (i = 0; i < lexer->buf_size; i++) {
        token_destroy(lexer->token_buffer[i]);
    }

    free(lexer->token_buffer);
    lexer->token_buffer = NULL;

    lexer->buf_size = 0;
    lexer->buf_capacity = 0;

    free(lexer);
    lexer = NULL;
}

struct token *token_create(struct token *token, enum token_type type) {
    token = malloc(sizeof(*token));
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
    default: {
        break;
    }
    }
    return token;
}

void token_destroy(struct token *token) {
    free(token);
    token = NULL;
}
