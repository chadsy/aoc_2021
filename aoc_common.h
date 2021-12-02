// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Common header for some macros and implementations.

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define countof(x)      (sizeof(x)/sizeof(x[0]))

typedef struct runargs_t {
    FILE *input;
    FILE *output;
    bool run_first;
    bool run_second;
} runargs;

static char *trim(char *str) {
    char *p = str + (strlen(str) - 1);
    while (isspace(*p)) {
        *p-- = '\0';
    }
    return str;
}

static runargs parse_args(int argc, char **argv) {
    runargs args;
    args.run_first = true;
    args.run_second = true;
    args.input = stdin;
    args.output = stdout;

    for (int i = 1; i < argc; i++) {
        // If this is a single char, then it should be 1 or 2 for first or second pass
        if (strlen(argv[i]) == 1) {
            if (*argv[i] == '1') {
                args.run_first = true;
                args.run_second = false;
            }
            else if (*argv[1] == '2') {
                args.run_second = true;
                args.run_first = false;
            }
            else {
                fprintf(stderr, "error: unknown pass specifier '%c'; ignoring.\n", *argv[i]);
                args.run_first = true;
                args.run_second = true;
            }
        }
        else {
            // Otherise, this is the input filename
            FILE *f = fopen(trim(argv[i]), "r");
            if (f) {
                args.input = f;
            }
            else {
                fprintf(stderr, "error: cannot open '%s', %s; using stdin instead.\n", argv[i], strerror(errno));
                args.input = stdin;
            }
        }
    }

    return args;
}