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

#define MAX_BUFFER 1024

static void repl() {
    char line[MAX_BUFFER];
    for (;;) {
        printf(DIM "(Bitter) $ " RESET);

        if (!fgets(line, sizeof(line), stdin)) {
            putchar('\n');
            break;
        }

        if (line[0] == ':') {
            if (line[1] == 'q') {
                if (line[2] == 'u') {
                    if (line[3] == 'i') {
                        if (line[4] == 't') {
                            exit(EXIT_SUCCESS);
                        }
                    }
                }
            }
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

    /* MEMORY DEBUGGING - DO NOT TOUCH */
#ifdef F_MEMORY_DEBUG
    f_debug_memory_init(NULL, NULL, NULL);
    f_debug_mem_print(0);
    f_debug_mem_reset();
#endif

    return EXIT_SUCCESS;
}
