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

#define MAX_BUFFER INT_MAX
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

    lexer->size = DEFAULT_BUFFER;
    lexer->source = malloc(1 + DEFAULT_BUFFER * sizeof(*lexer->source));

    lexer->token_buffer = malloc(DEFAULT_BUFFER * sizeof(*lexer->token_buffer));

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

    if (vm.lexer->size != src_len) {
        vm.lexer->size = src_len;
        vm.lexer->source = realloc(
            vm.lexer->source, 1 + vm.lexer->size * sizeof(*vm.lexer->source));
        vm.lexer->token_buffer =
            realloc(vm.lexer->token_buffer,
                    vm.lexer->size * sizeof(*vm.lexer->token_buffer));
    }

    memcpy(vm.lexer->source, source, vm.lexer->size);
    vm.lexer->source[vm.lexer->size] = '\0';
}

static char *sanitize_source(char *dest, char *source) {
    size_t i, j;
    int src_len;
    src_len = strlen(source);
    char valid_src[src_len + 1];
    for (i = j = 0; i < src_len; i++) {
        if (source[i] == '>' || source[i] == '<' || source[i] == '(' ||
            source[i] == ')' || source[i] == '!' || source[i] == '#') {
            valid_src[j++] = source[i];
        }
    }
    src_len = j;
    valid_src[j] = '\0';
    dest = malloc(1 + src_len * sizeof(*dest));

    memcpy(dest, valid_src, src_len);
    dest[src_len] = '\0';

    return dest;
}

static void validate_parentheses(char *source) {
    size_t i, j;
    int src_len;
    src_len = strlen(source);
    char parens[src_len + 1];

    for (i = j = 0; i < src_len; i++) {
        if (source[i] == '(' || source[i] == ')') {
            parens[j++] = source[i];
        }
    }

    src_len = j;
    parens[src_len] = '\0';

    char buffer[src_len + 1];
    buffer[src_len] = '\0';
    for (i = j = 0; i < src_len; i++) {
        if (parens[i] == '(') {
            buffer[j++] = parens[i];
        } else {
            if (j <= 0) {
                fprintf(stderr, "SYNTAX ERROR: Invalid parentheses.\n");
                exit(EXIT_FAILURE);
                // return false;
            }
            j--;
        }
    }
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
            break;
        }
        }
        lexer->token_buffer[i] = token;
    }
}

void execute(void) {

    char invert_bit[2] = {[0] = 1, [1] = 0};

    vm.highest_data_pointer = vm.data_pointer;

    // while (vm.instruction_pointer >= 0 &&
    //            vm.instruction_pointer < vm.lexer->size ||
    //        vm.data_pointer >= 0 && vm.data_pointer < vm.lexer->size) {
    while (true) {
        if (vm.instruction_pointer < 0 ||
            vm.instruction_pointer >= vm.lexer->size || vm.data_pointer < 0 ||
            vm.data_pointer >= vm.lexer->size) {
            return;
        }
        switch (vm.lexer->token_buffer[vm.instruction_pointer]->type) {
        case (GREATER): {
            vm.data_pointer++;
            vm.highest_data_pointer =
                max(vm.data_pointer, vm.highest_data_pointer);
            vm.data->buffer[vm.data_pointer] =
                invert_bit[vm.data->buffer[vm.data_pointer]];
            vm.instruction_pointer++;
            break;
        }
        case (LESS): {
            vm.highest_data_pointer =
                max(vm.data_pointer, vm.highest_data_pointer);
            vm.data_pointer--;
            vm.data->buffer[vm.data_pointer] =
                invert_bit[vm.data->buffer[vm.data_pointer]];
            vm.instruction_pointer++;
            break;
        }
        case (OPEN_PAREN): {
            if (vm.data->buffer[vm.data_pointer] == 1) {
                vm.instruction_pointer++;
            } else {
                // TODO: find next close paren + 1
                vm.instruction_pointer++; // FIX: replace this placeholder
            }
            break;
        }
        case (CLOSE_PAREN): {
            // TODO: find previous open paren
            vm.instruction_pointer++; // FIX: replace this placeholder
            break;
        }
        case (BANG): {
            size_t i;
            for (i = 0; i <= vm.highest_data_pointer; i++) {
                printf("%d ", vm.data->buffer[i]);
            }
            putchar('\n');
            vm.instruction_pointer++;
            break;
        }
        case (HASH): {
            size_t i;
            for (i = 0; i < vm.data->size; i++) {
                printf("%d ", vm.data->buffer[i]);
            }
            putchar('\n');
            vm.instruction_pointer++;
            break;
        }
        default: {
            break;
        }
        }
    }
}

// TODO: a function that finds all parentheses and stores their matches
// locations

static void bin_to_char(void) {
    size_t i;
    size_t j = 0;
    int buffer[8];
    for (i = 0; i < vm.highest_data_pointer; i++) {
        buffer[j++] = vm.data->buffer[i];
        if (j == 8) {
            int temp = 0;
            temp += buffer[0] & 1;
            temp <<= 1;
            temp += buffer[1] & 1;
            temp <<= 1;
            temp += buffer[2] & 1;
            temp <<= 1;
            temp += buffer[3] & 1;
            temp <<= 1;
            temp += buffer[4] & 1;
            temp <<= 1;
            temp += buffer[5] & 1;
            temp <<= 1;
            temp += buffer[6] & 1;
            temp <<= 1;
            temp += buffer[7] & 1;
            dbg(buffer);
            dbg(temp);
            printf("%c", temp);
            j = 0;
        }
    }
}

void run(char *source) {

    vm_create();

    char *san_src;
    san_src = sanitize_source(san_src, source);

    validate_parentheses(san_src);

    lexer_init(san_src);

    tokenize(vm.lexer);
    execute();
    bin_to_char();

    free(san_src);
    vm_destroy();
}
