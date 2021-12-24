// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 9 - Smoke Basin
// The one about smoke flowing downhill to the lowest points in the caves.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS    100
#define MAX_COLUMNS  100

char *map;
int cols = 0, rows = 0;

#define _COLS       cols
#define _MAP        map
#define _ROWS       rows
#define AT(r,c)     (_MAP[(r) * _COLS + (c)])
// #define ROW(r)      (SYM_MAP[(r) * SYM_COLS])
#define ROW_FOR(n)  ((n) / _COLS)
#define COL_FOR(n)  ((n) % _COLS)

bool is_lowest(int n) {
    int higher = 0;

    // 'higher' is the number of adjacent neighbors that are greater than the
    // item under considerat at map[r,c]. So after evaluating everyone,
    // if 'higher' == 4, then we have the lowest.

    int r = ROW_FOR(n);
    int c = COL_FOR(n);
    // printf("examining %d at %d,%d: %d\n", n, r, c, _MAP[n]);

    // First, adjust for the edges
    if (r == 0)
        higher++;
    if (r == (_ROWS - 1))
        higher++;
    if (c == 0)
        higher++;
    if (c == (_COLS - 1))
        higher++;

    char t = AT(r,c);

    if (r > 0 && AT(r-1,c) > t)
        higher++;
    if (r < _ROWS - 1 && AT(r+1,c) > t)
        higher++;
    if (c > 0 && AT(r,c-1) > t)
        higher++;
    if (c < _COLS - 1 && AT(r,c+1) > t)
        higher++;

    return higher == 4;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    map = calloc(MAX_ROWS * MAX_COLUMNS, sizeof(char));
    char *curmap = map;

    while (fgets(buf, sizeof(buf) - 1, args.input)) {
        trim(buf);
        if (!cols)
            cols = strlen(buf);
        char *p = buf;
        while (*p) {
            *curmap = *p - '0';
            p++;
            curmap++;
        }

        rows++;
    }

    // for (int r = 0; r < rows; r++) {
    //     for (int c = 0; c < cols; c++)
    //         printf("%c", AT(r, c) + '0');
    //     printf("\n");
    // }

    if (args.run_first) {
        int risk = 0, low = 0;

        for (int i = 0; i < rows * cols; i++) {
            if (is_lowest(i)) {
                low++;
                risk += _MAP[i] + 1;
            }
        }

        printf("first, sum of all risk levels of %d low points is: %d\n", low, risk);
    }

    if (args.run_second) {
        // printf("second, product of position %d and depth %d (with aim calculation) is: %d\n", position, depth, position * depth);
    }

    return 0;
}