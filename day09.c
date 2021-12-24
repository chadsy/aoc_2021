// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 9 - Smoke Basin
// The one about smoke flowing downhill to the lowest points in the caves.
//
// NB: because reasons, I ended up with global variables for the map, rows, cols.
//
// This seems to be pretty straightforward, finding the height of the lowest
// point that is surrounded by higher points (or edges). Working only on regular
// axes (not diagonals), we can just check the neighbors of each point. If the
// point has 4 higher neighbors, it's a low point and counts. Then we add up,
// adding 1 to each (also because reasons). It's called 'risk', look it up.
//
// Part 2 is to extend the low points to a basin, and count the number of points
// on the map that are bounded by points of height '9'. So start with a low and
// keep walking in all directions (recursive?) until we hit a '9'. Count the
// number of items in that bounded area. Then, at the end, multiply the 3 largest
// basin sizes and report the product.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS    100
#define MAX_COLUMNS  100

char *map;
int cols = 0, rows = 0;

// I'm a sucker for pointer math and global variables
#define _COLS       cols
#define _MAP        map
#define _ROWS       rows
#define AT(r,c)     (_MAP[(r) * _COLS + (c)])
#define ROW_FOR(n)  ((n) / _COLS)
#define COL_FOR(n)  ((n) % _COLS)

bool is_lowest(int n) {
    int higher = 0;

    // 'higher' is the number of adjacent neighbors that are greater than the
    // item under considerat at map[r,c]. So after evaluating everyone,
    // if 'higher' == 4, then we have the lowest.

    int r = ROW_FOR(n);
    int c = COL_FOR(n);

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

int sum_basin(int n) {
    // 9 is the boundary of a basin.
    if (_MAP[n] == 9)
        return 0;

    // using -1 to indicate that we've already visited this location; without
    // this check, we overflow the stack. Duh.
    if (_MAP[n] == -1)
        return 0;

    _MAP[n] = -1;
    int sum = 1;

    if (ROW_FOR(n) > 0)
        sum += sum_basin(n - _COLS);
    if (ROW_FOR(n) < (_ROWS - 1))
        sum += sum_basin(n + _COLS);
    if (COL_FOR(n) > 0)
        sum += sum_basin(n - 1);
    if (COL_FOR(n) < (_COLS - 1))
        sum += sum_basin(n + 1);

    return sum;
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
        int b1 = 1, b2 = 1, b3 = 1;

        for (int i = 0; i < rows * cols; i++) {
            if (is_lowest(i)) {
                // using a lowest point, go size up the basin
                int size = sum_basin(i);
                if (size > b1) {
                    b3 = b2;
                    b2 = b1;
                    b1 = size;
                }
                else if (size > b2) {
                    b3 = b2;
                    b2 = size;
                }
                else if (size > b3) {
                    b3 = size;
                }
            }
        }

        printf("second, product of 3 largest basins is: %d\n", b1 * b2 * b3);
    }

    return 0;
}