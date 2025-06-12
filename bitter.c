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

void error_print(const char *file, const unsigned line, const char *function,
                 const char *format, ...) {

    time_t now;
    char *time_str;
    now = time(NULL);
    time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';

    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s%s[%s] %s:%d:%s()%s %s%sERROR:%s ", DEFAULT, DIM,
            time_str, file, line, function, RESET, BRIGHT_RED, BOLD, RESET);
    fprintf(stderr, "%s", BOLD);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s", RESET);
    va_end(args);
}

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
    if (!data) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    data->size = DEFAULT_BUFFER;
    data->capacity = DEFAULT_BUFFER * 2;
    data->buffer = malloc(data->capacity * sizeof(*data->buffer));

    if (!data->buffer) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

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

    if (!lexer) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    lexer->size = DEFAULT_BUFFER;
    lexer->source = malloc(1 + DEFAULT_BUFFER * sizeof(*lexer->source));

    if (!lexer->source) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    lexer->buffer = malloc(DEFAULT_BUFFER * sizeof(*lexer->buffer));

    if (!lexer->buffer) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    return lexer;
}

void lexer_destroy(struct lexer *lexer) {
    size_t i;

    free(lexer->source);
    lexer->source = NULL;

    for (i = 0; i < lexer->size; i++) {
        token_destroy(lexer->buffer[i]);
    }

    free(lexer->buffer);
    lexer->buffer = NULL;

    lexer->size = 0;

    free(lexer);
    lexer = NULL;
}

struct token *token_create(struct token *token, enum token_type type,
                           int location) {
    token = malloc(sizeof(*token));

    if (!token) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    switch (type) {
    case (GREATER): {
        token->type = GREATER;
        token->literal = '>';
        token->location = location;
        break;
    }
    case (LESS): {
        token->type = LESS;
        token->literal = '<';
        token->location = location;
        break;
    }
    case (OPEN_PAREN): {
        token->type = OPEN_PAREN;
        token->literal = '(';
        token->location = location;
        break;
    }
    case (CLOSE_PAREN): {
        token->type = CLOSE_PAREN;
        token->literal = ')';
        token->location = location;
        break;
    }
    case (BANG): {
        token->type = BANG;
        token->literal = '!';
        token->location = location;
        break;
    }
    case (HASH): {
        token->type = HASH;
        token->literal = '#';
        token->location = location;
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

        if (!vm.lexer->source) {
            ERROR("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        vm.lexer->buffer = realloc(vm.lexer->buffer,
                                   vm.lexer->size * sizeof(*vm.lexer->buffer));

        if (!vm.lexer->buffer) {
            ERROR("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
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

    if (!dest) {
        ERROR("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

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
                ERROR("SYNTAX ERROR: Invalid parentheses.\n");
                exit(EXIT_FAILURE);
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
            token = token_create(token, GREATER, i);
            break;
        }
        case ('<'): {
            token = token_create(token, LESS, i);
            break;
        }
        case ('('): {
            token = token_create(token, OPEN_PAREN, i);
            break;
        }
        case (')'): {
            token = token_create(token, CLOSE_PAREN, i);
            break;
        }
        case ('!'): {
            token = token_create(token, BANG, i);
            break;
        }
        case ('#'): {
            token = token_create(token, HASH, i);
            break;
        }
        default: {
            break;
        }
        }
        lexer->buffer[i] = token;
    }
}

static void match_parens(void) {
    size_t i, j;
    struct token *buffer[vm.lexer->size];

    for (i = j = 0; i < vm.lexer->size; i++) {
        if (vm.lexer->buffer[i]->type == OPEN_PAREN) {
            buffer[j++] = vm.lexer->buffer[i];
        } else {
            if (vm.lexer->buffer[i]->type == CLOSE_PAREN) {
                buffer[--j]->match_location = vm.lexer->buffer[i]->location;
                vm.lexer->buffer[i]->match_location = buffer[j]->location;
            }
        }
    }
}

static void memory_check() {
    if (vm.data->size == vm.data->capacity) {
        vm.data->capacity *= 2;
        vm.data = realloc(vm.data, vm.data->capacity * sizeof(*vm.data));
        if (!vm.data) {
            ERROR("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

static bool bounds_check() {
    if (vm.instruction_pointer < 0 ||
        vm.instruction_pointer >= vm.lexer->size || vm.data_pointer < 0) {
        return false;
    }
    return true;
}

static void memory_dump(void) {
    size_t i, j;
    size_t size;
    size = CEIL_DIV(vm.highest_data_pointer, 8);
    int buffer[size];
    for (i = 0; i < size; i++) {
        buffer[i] = 0;
    }

    for (i = j = 0; i <= vm.highest_data_pointer; i++) {
        buffer[j] <<= 1;
        buffer[j] += vm.data->buffer[i];
        if ((i + 1) % 8 == 0) {
            j++;
        }
    }

    puts("========================================");
    puts("Memory Dump: ");
    puts("========================================");

    puts("--------------------");
    puts("Tape (Bits)");
    puts("--------------------");
    for (i = 0; i <= vm.highest_data_pointer; i++) {
        printf("%d ", vm.data->buffer[i]);
        if ((i + 1) % 8 == 0) {
            putchar('\n');
        }
    }
    putchar('\n');

    puts("--------------------");
    puts("Tape (Char)");
    puts("--------------------");
    for (i = 0; i < size; i++) {
        printf("%c", buffer[i]);
    }
    putchar('\n');

    puts("--------------------");
    puts("Tape (Int)");
    puts("--------------------");
    for (i = 0; i < size; i++) {
        printf("%d ", buffer[i]);
    }
    putchar('\n');

    puts("--------------------");
    puts("Tape (Hex)");
    puts("--------------------");
    for (i = 0; i < size; i++) {
        printf("%x ", buffer[i]);
    }
    putchar('\n');

    puts("--------------------");
    puts("Tape (Oct)");
    puts("--------------------");
    for (i = 0; i < size; i++) {
        printf("%o ", buffer[i]);
    }
    putchar('\n');
    puts("========================================");
}

void execute(void) {

    char invert_bit[2] = {[0] = 1, [1] = 0};

    vm.highest_data_pointer = vm.data_pointer;

    while (true) {
        if (!bounds_check()) {
            return;
        }
        switch (vm.lexer->buffer[vm.instruction_pointer]->type) {
        case (GREATER): {
            if (vm.data_pointer == vm.data->size) {
                vm.data->size++;
            }
            vm.data_pointer++;

            memory_check();

            if (!bounds_check()) {

                ERROR("Out of bounds error.\n");
                exit(EXIT_FAILURE);
            }
            vm.highest_data_pointer =
                MAX(vm.data_pointer, vm.highest_data_pointer);
            vm.data->buffer[vm.data_pointer] =
                invert_bit[vm.data->buffer[vm.data_pointer]];
            vm.instruction_pointer++;
            break;
        }
        case (LESS): {
            vm.highest_data_pointer =
                MAX(vm.data_pointer, vm.highest_data_pointer);
            vm.data_pointer--;

            if (!bounds_check()) {
                ERROR("Out of bounds error.\n");
                exit(EXIT_FAILURE);
            }
            vm.data->buffer[vm.data_pointer] =
                invert_bit[vm.data->buffer[vm.data_pointer]];
            vm.instruction_pointer++;
            break;
        }
        case (OPEN_PAREN): {
            if (vm.data->buffer[vm.data_pointer] != 1) {
                vm.instruction_pointer =
                    vm.lexer->buffer[vm.instruction_pointer]->match_location;
            }
            vm.instruction_pointer++;

            if (!bounds_check()) {
                // ERROR("Out of bounds error.\n");
                return;
            }

            break;
        }
        case (CLOSE_PAREN): {
            vm.instruction_pointer =
                vm.lexer->buffer[vm.instruction_pointer]->match_location;

            if (!bounds_check()) {
                // ERROR("Out of bounds error.\n");
                return;
            }
            break;
        }

        case (BANG): {
            size_t i;
            for (i = 0; i <= vm.highest_data_pointer; i++) {
                printf("%d ", vm.data->buffer[i]);
                if ((i + 1) % 8 == 0) {
                    putchar('\n');
                }
            }
            putchar('\n');
            vm.instruction_pointer++;
            break;
        }

        case (HASH): {
            memory_dump();
            vm.instruction_pointer++;
            break;
        }
        default: {
            break;
        }
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

    match_parens();

    execute();

    free(san_src);
    vm_destroy();
}
