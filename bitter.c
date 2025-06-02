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

Tape *data;
Tape *input;

void memory_create(void) {
    data = malloc(sizeof(*data));
    input = malloc(sizeof(*input));

    data->size = MAX_BUFFER;
    input->size = MAX_BUFFER;

    data->buffer = malloc(data->size * sizeof(*data->buffer));
    input->buffer = malloc(input->size * sizeof(*input->buffer));

    data->pointer = 0;
    input->pointer = 0;

    data->highest_pointer = data->pointer;

    memset(data->buffer, ZERO, data->size);
    memset(input->buffer, ZERO, input->size);
}

void memory_destroy(void) {
    free(data->buffer);
    free(input->buffer);
    free(data);
    free(input);
}

uint8_t invert_bit(uint8_t bit) {
    if (bit == ZERO)
        return ONE;
    return ZERO;
}

void lexer(char *str, size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        input->buffer[i] = str[i];
    }
    execute();
    memory_dump(MAX_BUFFER);
}

bool check(void) {
    if (input->pointer >= 0 && input->pointer < MAX_BUFFER &&
        data->pointer >= 0 && data->pointer < MAX_BUFFER) {
        return true;
    }
    return false;
}

bool increment(void) {
    data->pointer++;
    data->highest_pointer = MAX(size_t, data->highest_pointer, data->pointer);
    if (check()) {
        uint8_t temp = data->buffer[data->pointer];
        data->buffer[data->pointer] = invert_bit(temp);
        unsigned char a = temp;
        input->pointer++;
        return true;
    }
    return false;
}

bool decrement(void) {
    data->highest_pointer = MAX(size_t, data->highest_pointer, data->pointer);
    data->pointer--;
    if (check()) {
        uint8_t temp = data->buffer[data->pointer];
        data->buffer[data->pointer] = invert_bit(temp);
        unsigned char a = temp;
        input->pointer++;
        return true;
    }
    return false;
}

void execute(void) {
    while (input->buffer[input->pointer] && check()) {
        if (input->buffer[input->pointer] == TOKEN_INC) {
            if (!increment()) {
                return;
            }
        } else if (input->buffer[input->pointer] == TOKEN_DEC) {
            if (!decrement()) {
                return;
            }
        } else if (input->buffer[input->pointer] == TOKEN_DUMP_TO_DATA_PTR) {
            memory_dump(data->highest_pointer + 1);
            input->pointer++;
        } else if (input->buffer[input->pointer] ==
                   TOKEN_PAUSE_AND_DUMP_MEM_FULL) {
            memory_dump(MAX_BUFFER);
            input->pointer++;
        }
    }
}

void memory_dump(size_t size) {
    size_t i;
    for (i = 0; i < size; i++) {
        printf("%u ", data->buffer[i]);
    }
    putchar('\n');
}
