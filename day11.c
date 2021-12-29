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
int increment_neighbors(char *octos, int idx);
void dump_octos(char *octos);

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    int rows = 0;
    char octos[OCTO_COUNT] = {};

    while (rows < OCTO_ROWS && fgets(buf, sizeof(buf) - 1, args.input)) {
        for (int c = 0; c < OCTO_COLS; c++)
            buf[c] -= '0';
        memmove(octos + (rows * OCTO_COLS), buf, OCTO_COLS);
        rows++;
    }

    printf("--- start ---\n");
    dump_octos(octos);

    if (args.run_first) {
        int flashes = 0;
        int steps = 10;   // ENERGY_STEPS;

        for (int step = 1; step <= steps; step++) {
            for (int i = 0; i < OCTO_COUNT; i++)
                octos[i]++;

            int fl = 0;
            for (int i = 0; i < OCTO_COUNT; i++) {
                if (octos[i] == 10)
                    fl += increment_neighbors(octos, i);
            }

            printf("--- step %d ---\n", step);
            // int fl = count_flashes(octos);
            flashes += fl;
            dump_octos(octos);
            printf("  %d flashes\n", fl);
            reset_flashes(octos);
            // dump_octos(octos);
            // printf("\n");
        }

        printf("first, number of flashes after %d steps: %d\n", steps, flashes);
    }

    if (args.run_second) {
        // printf("second, product of position %d and depth %d (with aim calculation) is: %d\n", position, depth, position * depth);
    }

    return 0;
}

int increment_neighbors(char *octos, int idx) {
    int r = idx / OCTO_COLS;
    int c = idx % OCTO_COLS;
    int flashes = 0;

    for (int ry = max(0, r - 1); ry <= min(OCTO_ROWS, r + 1); ry++) {
        for (int cx = max(0, c - 1); cx <= min(OCTO_COLS, c + 1); cx++) {
            if (ry == 0 && cx == 0)
                continue;

            AT(octos, ry, cx)++;

            if (AT(octos, ry, cx) == 10) {
                flashes++;
                flashes += increment_neighbors(octos, SUBSCRIPT(ry, cx));
            }
        }
    }

    return flashes;

    // for (int i = -1; i <= 1; i++) {
    //     for (int j = -1; j <= 1; j++) {
    //         if (i == 0 && j == 0)
    //             continue;

    //         if (r + i < 0 ||
    //             c + j < 0 ||
    //             r + i >= OCTO_ROWS ||
    //             c + j >= OCTO_COLS)
    //             continue;
    //         AT(octos, r + i, c + j)++;

    //         if (AT(octos, r + i, c + j) > 9)
    //             increment_neighbors(octos, SUBSCRIPT(r + i, c + j));
    //     }
    // }
    // prev row
   //  if (r > 0) {
   //      AT(octos, r - 1, c)++;
   //      if (c > 0)                  AT(octos, r - 1, c - 1)++;
   //      if (c < OCTO_COLS - 1)      AT(octos, r - 1, c + 1)++;
   //  }

   //   // main row
   //  if (c > 0)                      AT(octos, r, c - 1)++;
   //  if (c < OCTO_COLS - 1)          AT(octos, r, c + 1)++;

   // // next row
   //  if (r < OCTO_ROWS - 1) {
   //      AT(octos, r + 1, c)++;
   //      if (c > 0)                  AT(octos, r + 1, c - 1)++;
   //      if (c < OCTO_COLS - 1)      AT(octos, r + 1, c + 1)++;
   //  }
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
