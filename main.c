/* File: main.c
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

static void truth_machine(void) {
    char *source;

    // Memory: 001
    source = ">><!";

    // WARNING: uncommenting the following line will cause the program to enter
    // an infinite loop that turn all memory into 1

    // Memory: 1111111111111111111111...
    // source = "><(!>><)";

    run(source);
}

static void test0(void) {
    char *source;

    // 011111
    source = ">>>>>!";
    run(source);
}

static void test1(void) {
    char *source;

    // Out of bounds error
    source = "<<<<<!";

    run(source);
}

static void test2(void) {
    char *source;
    source = ">(<)!";
    run(source);
}

static void test3(void) {
    char *source;
    source = ">><(><)";
    run(source);
}

static void test4(void) {
    char *source;
    source = "(>><)>><!";
    run(source);
}

static void test5(void) {
    char *source;
    source = ">><((>><)>><)!";
    run(source);
}

static void test6(void) {
    char *source = "<";
    run(source);
}

static void test7(void) {
    char *source = ">>><>>><>>><>>><>>><!";
    run(source);
}

static void test8(void) {
    char *source = ">>><>>><>>><!";
    run(source);
}

static void test9(void) {
    char *source = "!";
    run(source);
}

static void repl() {
    char line[DEFAULT_BUFFER];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        run(line);
    }
}

static char *read_file(const char *filepath) {

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        ERROR("Could not open file \"%s\".\n", filepath);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    // fseek(file, 0L, SEEK_SET);
    rewind(file);

    char *buffer = malloc(1 + size * sizeof(*buffer));
    if (!buffer) {
        ERROR("Failed to read memory for \"%s\".\n", filepath);
        exit(EXIT_FAILURE);
    }

    size_t bytes = fread(buffer, sizeof(char), size, file);
    if (bytes < size) {
        ERROR("Failed to read file \"%s\".\n", filepath);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';

    fclose(file);
    return buffer;
}

static void run_file(const char *path) {
    char *source = read_file(path);
    run(source);
    free(source);
}

int main(int argc, char **argv) {

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        ERROR("Usage: bitter [path]\n");
        exit(EXIT_FAILURE);
    }
    // TODO: example uses

    /* MEMORY DEBUGGING - DO NOT TOUCH */
    f_debug_memory_init(NULL, NULL, NULL);
    f_debug_mem_print(0);
    f_debug_mem_reset();

    return EXIT_SUCCESS;
}
