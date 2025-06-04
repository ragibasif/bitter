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

// Bitter commands:

// >
// increment data pointer and invert bit
//
// <
// decrement data pointer and invert bit
//
// (
// if [p] == 1, proceed to next command, otherwise advance to command after
// matching ')'
//
// )
// go back to matching (
//
// !
// interpreter command: dump memory from 0 to highest value of chunkta pointer
//
// #
// interpreter command: pause program and dump memory

#include "bitter.h"

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

void chunk_create(Chunk *chunk) {
    chunk->size = 0;
    chunk->capacity = 0;
    chunk->array = NULL;
}

void chunk_write(Chunk *chunk, uint8_t byte) {
    if (chunk->capacity < chunk->size + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->array =
            GROW_ARRAY(uint8_t, chunk->array, oldCapacity, chunk->capacity);
    }

    chunk->array[chunk->size] = byte;
    chunk->size++;
}

void chunk_destroy(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->array, chunk->capacity);
    chunk_create(chunk); // zero out the fields to leave the chunk in a well
                         // defined empty state
}

void disassemble_chunk(Chunk *chunk, const char *name) {
    dbg(name);
    for (int offset = 0; offset < chunk->size;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int simple_instruction(const char *name, int offset) {
    dbg(name);
    return offset + 1;
}

int disassemble_instruction(Chunk *chunk, int offset) {
    dbg(offset);

    uint8_t instruction = chunk->array[offset];
    switch (instruction) {
    case OP_RETURN:
        return simple_instruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
