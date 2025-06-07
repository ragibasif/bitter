/* File: main.c
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

// Bitter commands:

// >
// increment data pointer and invert bit

// <
// decrement data pointer and invert bit

// (
// if [p] == 1, proceed to next command, otherwise advance to command after
// matching ')'

// )
// go back to matching (

// !
// interpreter command: dump memory from 0 to highest value of data pointer

// #
// interpreter command: pause program and dump memory

#include "bitter.h"

enum token_type {
    GREATER,     // >
    LESS,        // <
    OPEN_PAREN,  // (
    CLOSE_PAREN, // )
    BANG,        // !
    HASH,        // #
};

struct token {
    enum token_type type;
    char literal;
};

void dbg_token(struct token *token) {
    dbg(token);
    dbg(token->type);
    dbg(token->literal);
}

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
    dbg_token(token);
}

#define MAX_BUFFER 10

struct lexer {
    char *source;
    struct token token_buffer[MAX_BUFFER];
};

void dbg_lexer(struct lexer *lexer) {
    dbg(&lexer);
    dbg(lexer);
    dbg(&lexer->source);
    dbg(lexer->source);
    dbg(&lexer->token_buffer);
    dbg(lexer->token_buffer);
    size_t i;
    for (i = 0; i < 2; i++) {
        // *(((int *)array->buffer) + i) = i + 100;
        dbg(&*(((struct token *)lexer->token_buffer) + i));
        dbg((((struct token *)lexer->token_buffer) + i)->literal);
        dbg(*&(((struct token *)lexer->token_buffer) + i)->type);
    }
}

char data_buffer[MAX_BUFFER];

static void execute(struct lexer *lexer) {
    size_t ip;
    size_t dp;
    size_t op;
    size_t cp;
    ip = 0;
    dp = 0;
    for (ip = 0; ip < MAX_BUFFER; ip++) {
        printf("%c\n", lexer->token_buffer[ip].literal);
        switch (lexer->token_buffer[ip].type) {
        case (GREATER): {
            dp++;
            data_buffer[dp] = ~data_buffer[dp];
            break;
        }
        case (LESS): {
            dp--;
            data_buffer[dp] = ~data_buffer[dp];
            break;
        }
        case (OPEN_PAREN): {
            op = ip;
            if (!data_buffer[dp]) {
                ip = cp + 1;
            }
            break;
        }
        case (CLOSE_PAREN): {
            cp = ip;
            ip = op;
            break;
        }
        case (BANG): {
            dbg(data_buffer);
            break;
        }
        case (HASH): {
            dbg(data_buffer);
            break;
        }
        default: {
            break;
        }
        }
    }
    dbg(data_buffer[0]);
    dbg(data_buffer[1]);
    dbg(data_buffer[2]);
}

static void run(char *source) {
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
        c = source[i];
        struct token token;
        switch (c) {
        case ('>'):
            token_create(&token, GREATER);
            break;
        case ('<'):
            token_create(&token, LESS);
            break;
        case ('('):
            token_create(&token, OPEN_PAREN);
            break;
        case (')'):
            token_create(&token, CLOSE_PAREN);
            break;
        case ('!'):
            token_create(&token, BANG);
            break;
        case ('#'):
            token_create(&token, HASH);
            break;
        default:
            break;
        }
        lexer->token_buffer[i++] = token;
    }

    execute(lexer);

    dbg_lexer(lexer);
    free(lexer->source);
    lexer->source = NULL;
    free(lexer);
    lexer = NULL;
}

int main(int argc, char **argv) {
    run(">[[]]");

    /* MEMORY DEBUGGING - DO NOT TOUCH */
    f_debug_memory_init(NULL, NULL, NULL);
    f_debug_mem_print(0);
    f_debug_mem_reset();

    return EXIT_SUCCESS;
}
