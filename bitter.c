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
    // vm.lexer->buf_size = 1;
    // struct token *token;
    // token = token_create(token, GREATER);
    // vm.lexer->token_buffer[0] = token;
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

    lexer->size = 0;
    lexer->source = NULL;

    lexer->token_buffer = NULL;

    return lexer;
}

void lexer_destroy(struct lexer *lexer) {
    size_t i;

    free(lexer->source);
    lexer->source = NULL;

    for (i = 0; i < lexer->size; i++) {
        token_destroy(lexer->token_buffer[i]);
    }

    free(lexer->token_buffer);
    lexer->token_buffer = NULL;

    lexer->size = 0;

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
        token->type = NONE;
        token->literal = '\0';
        break;
    }
    }
    return token;
}

void token_destroy(struct token *token) {
    free(token);
    token = NULL;
}

void lexer_init(char *source) {
    int src_len;
    src_len = strlen(source);
    if (src_len == 0) {
        return;
    }
    vm.lexer->size = src_len;
    vm.lexer->source = malloc(1 + vm.lexer->size * sizeof(*vm.lexer->source));
    memcpy(vm.lexer->source, source, vm.lexer->size);
    vm.lexer->source[vm.lexer->size] = '\0';

    vm.lexer->token_buffer =
        malloc(vm.lexer->size * sizeof(*vm.lexer->token_buffer));
}

void run(char *source) {

    vm_create();

    lexer_init(source);

    tokenize(vm.lexer);

    vm_destroy();
}

void tokenize(struct lexer *lexer) {
    size_t i;
    for (i = 0; i < lexer->size; i++) {
        struct token *token;
        switch (lexer->source[i]) {
        case ('>'): {
            token = token_create(token, GREATER);
            break;
        }
        case ('<'): {
            token = token_create(token, LESS);
            break;
        }
        case ('('): {
            token = token_create(token, OPEN_PAREN);
            break;
        }
        case (')'): {
            token = token_create(token, CLOSE_PAREN);
            break;
        }
        case ('!'): {
            token = token_create(token, BANG);
            break;
        }
        case ('#'): {
            token = token_create(token, HASH);
            break;
        }
        default: {
            token = token_create(token, NONE);
            break;
        }
        }
        lexer->token_buffer[i] = token;
    }
}
