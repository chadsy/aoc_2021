// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License https://opensource.org/licenses/MIT
//
// Day 11 - Dumbo Octopus
// The one about siulating the energy in flashing octopuses.
//
// Starting down the usual path, a linear array logically broken up with pointer
// arithmetic. Makes for some terrible expressions, esp around the 'neighbor'
// calculations. So there's a bit of struggle with that stuff. That, and a
// possible overcomplication of the rules for applying a step.
//
// There's some trickiness in using recursion to incrementally light up the
// neighbors. Lots of different algos and factoring and attempts at simplifcation
// that didn't pan out. Finally got it.
//
// Part 2 is just like 1 in terms of process, except instead of counting flashes
// for 100 steps, let the steps run and count them until all octopuses fire at
// once, meaning the per-step flash count == 100. That's pretty simple after
// getting the flash logic right in the first part. Whee....

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define ENERGY_STEPS    100
#define OCTO_ROWS       10
#define OCTO_COLS       10
#define OCTO_COUNT      (OCTO_ROWS * OCTO_COLS)

#define SUBSCRIPT(r, c)     (((r) * OCTO_COLS) + (c))
#define AT(octos, r, c)     (octos[SUBSCRIPT((r), (c))])

void reset_flashes(char *octos);
void increment_neighbors(char *octos, int idx);
void dump_octos(char *octos);

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    int rows = 0;
    char octos[OCTO_COUNT] = {};
    char org_octos[OCTO_COUNT] = {};

    while (rows < OCTO_ROWS && fgets(buf, sizeof(buf) - 1, args.input)) {
        for (int c = 0; c < OCTO_COLS; c++)
            buf[c] -= '0';
        memcpy(octos + (rows * OCTO_COLS), buf, OCTO_COLS);
        rows++;
    }

    memcpy(org_octos, octos, sizeof(org_octos));

    if (args.run_first) {
        int flashes = 0;
        int steps = ENERGY_STEPS;

        for (int step = 0; step < steps; step++) {
            int fl = 0;

            for (int i = 0; i < OCTO_COUNT; i++) {
                octos[i]++;
                if (octos[i] == 10) {
                    increment_neighbors(octos, i);
                }
            }

            for (int i = 0; i < OCTO_COUNT; i++) {
                if (octos[i] > 9)
                    fl++;
            }

            flashes += fl;

            reset_flashes(octos);
        }

        printf("first, number of flashes after %d steps: %d\n", steps, flashes);
    }

    memcpy(octos, org_octos, sizeof(octos));

    if (args.run_second) {
        int all_flashed_at = 0;

        for (int step = 1; ; step++) {
            int fl = 0;

            for (int i = 0; i < OCTO_COUNT; i++) {
                octos[i]++;
                if (octos[i] == 10) {
                    increment_neighbors(octos, i);
                }
            }

            for (int i = 0; i < OCTO_COUNT; i++) {
                if (octos[i] > 9)
                    fl++;
            }

            if (fl == 100) {
                all_flashed_at = step;
                break;
            }

            reset_flashes(octos);
        }

        printf("second, the step where all octos flashed: %d\n", all_flashed_at);
    }

    return 0;
}

void increment_neighbors(char *octos, int idx) {
    int r = idx / OCTO_COLS;
    int c = idx % OCTO_COLS;

    for (int ry = -1; ry <= 1; ry++) {
        for (int cx = -1; cx <= 1; cx++) {
            if (ry == 0 && cx == 0)
                continue;
            if (r + ry < 0 || r + ry >= OCTO_ROWS)
                continue;
            if (c + cx < 0 || c + cx >= OCTO_COLS)
                continue;

            AT(octos, r + ry, c + cx)++;

            if (AT(octos, r + ry, c + cx) == 10) {
                increment_neighbors(octos, SUBSCRIPT(r + ry, c + cx));
            }
        }
    }
}

void reset_flashes(char *octos) {
    for (int i = 0; i < OCTO_COUNT; i++)
        if (octos[i] > 9)
            octos[i] = 0;
}

void dump_octos(char *octos) {
    char *digits = "01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int r = 0; r < OCTO_ROWS; r++) {
        char buf[OCTO_COLS + 1] = {};
        memmove(buf, octos + (r * OCTO_COLS), OCTO_COLS);
        for (int c = 0; c < OCTO_COLS; c++) {
            if (buf[c] == 0 || buf[c] > 9) {
                printf("\e[33;1m%c\e[m", digits[(int)buf[c]]);
            }
            else {
                printf("%c", buf[c] + '0');
            }
        }
        printf("\n");
    }
}
