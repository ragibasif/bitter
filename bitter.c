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

void commands_create(Commands *commands) {
    commands->code = NULL;
    commands->size = 0;
    commands->capacity = 0;
}

void commands_write(Commands *commands, uint8_t byte) {
    if (commands->capacity < commands->size + 1) {
        int oldCapacity = commands->capacity;
        commands->capacity = GROW_CAPACITY(oldCapacity);
        commands->code = GROW_ARRAY(uint8_t, commands->code, oldCapacity,
                                    commands->capacity);
    }

    commands->code[commands->size] = byte;
    commands->size++;
}

void commands_destroy(Commands *commands) {
    FREE_ARRAY(uint8_t, commands->code, commands->capacity);
    commands_create(commands);
}

void disassemble_commands(Commands *commands, const char *name) {
    dbg(name);
    for (int offset = 0; offset < commands->size;) {
        offset = disassemble_instruction(commands, offset);
    }
}

static int simple_instruction(const char *name, int offset) {
    dbg(name);
    return offset + 1;
}

// OP_INC_INVERT,    // > - move data ptr to the right and invert bit
// OP_DEC_INVERT,    // < - move data ptr to the left and invert bit
// OP_START_LOOP,    // ( - if [p] == 1 then next command, else matching ) + 1
// OP_END_LOOP,      // ) - back to matching (
// OP_USED_MEM_DUMP, // ! - dump mem from 0 to highest data ptr
// OP_FULL_MEM_DUMP, // # - pause program and dump full memory

int disassemble_instruction(Commands *commands, int offset) {
    dbg(offset);

    uint8_t instruction = commands->code[offset];
    switch (instruction) {
    case OP_INC_INVERT:
        return simple_instruction("OP_INC_INVERT", offset);
    case OP_DEC_INVERT:
        return simple_instruction("OP_DEC_INVERT", offset);
    case OP_START_LOOP:
        return simple_instruction("OP_START_LOOP", offset);
    case OP_END_LOOP:
        return simple_instruction("OP_END_LOOP", offset);
    case OP_USED_MEM_DUMP:
        return simple_instruction("OP_USED_MEM_DUMP", offset);
    case OP_FULL_MEM_DUMP:
        return simple_instruction("OP_FULL_MEM_DUMP", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}

void bit_array_create(BitArray *array) {
    array->bits = NULL;
    array->capacity = 0;
    array->size = 0;
}

void bit_array_write(BitArray *array, Bit bit) {
    if (array->capacity < array->size + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->bits =
            GROW_ARRAY(Bit, array->bits, oldCapacity, array->capacity);
    }

    array->bits[array->size] = bit;
    array->size++;
}

void bit_array_destroy(BitArray *array) {
    FREE_ARRAY(Bit, array->bits, array->capacity);
    bit_array_create(array);
}

VM vm;

static void reset_stack() { vm.stack_top = vm.stack; }

void vm_create() { reset_stack(); }
void vm_destroy() {}

#define DEBUG_TRACE_EXECUTION
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        disassemble_instruction(vm.commands, (int)(vm.ip - vm.commands->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
        case OP_INC_INVERT: {
            dbg(OP_INC_INVERT);
            break;
        }
        case OP_DEC_INVERT: {
            dbg(OP_DEC_INVERT);
            break;
        }
        case OP_START_LOOP: {
            dbg(OP_START_LOOP);
            break;
        }
        case OP_END_LOOP: {
            dbg(OP_END_LOOP);
            break;
        }
        case OP_USED_MEM_DUMP: {
            dbg(OP_USED_MEM_DUMP);
            break;
        }
        case OP_FULL_MEM_DUMP: {
            dbg(OP_FULL_MEM_DUMP);
            break;
        }
        case OP_RETURN: {
            return INTERPRET_OK;
        }
        default: {
            dbg("default");
            break;
        }
        }
    }
#undef READ_BYTE
}

InterpretResult interpret(const char *source) {
    compile(source);
    return INTERPRET_OK;
}

typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void scanner_create(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

void compile(const char *source) { scanner_create(source); }
