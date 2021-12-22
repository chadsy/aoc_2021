// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 5
// The one about the big grid of horz and vertical lines and counting the number
// of lines that hit each point.
//
// There really isn't much of a better way to solve this than creating a big-ass
// grid of shorts (chars?) and then for each vector just add one to the cell.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS    1000
#define MAX_COLS    1000
#define GRID_SIZE   (MAX_ROWS * MAX_COLS)

#define min(a, b)   ((a) > (b) ? (b) : (a))
#define max(a, b)   ((a) < (b) ? (b) : (a))

typedef struct {
    short x1, y1;
    short x2, y2;
} vector;

vector parse_line(char *line) {
    vector v = {0, 0, 0, 0};

    int count = sscanf(line, "%hd,%hd -> %hd,%hd", &v.x1, &v.y1, &v.x2, &v.y2);
    if (count != 4)
        fprintf(stderr, "error: couldn't parse input line '%s'\n", line);

    return v;
}

void apply_vector_regular(vector v, char *grid) {
    // only consider horizontal & vertical, so check that's true before
    // continuing to count these.

    // so if it's a horizontal line...
    if (v.x1 == v.x2) {
        for (int y = min(v.y1, v.y2); y <= max(v.y1, v.y2); y++) {
            grid[(y*MAX_COLS)+v.x1]++;
        }
    }
    // or if it's a vertical line...
    else if (v.y1 == v.y2) {
        for (int x = min(v.x1, v.x2); x <= max(v.x1, v.x2); x++) {
            grid[(v.y1*MAX_COLS)+x]++;
        }
    }
}

void apply_vector_diagonal(vector v, char *grid) {
    if (v.x1 != v.x2 && v.y1 != v.y2) {
        int ybias = (v.y1 > v.y2) ? -1 : 1;
        int xbias = (v.x1 > v.x2) ? -1 : 1;
        short x = v.x1;
        short y = v.y1;

        while (x != v.x2 && y != v.y2) {
            grid[(y*MAX_COLS)+x]++;
            x += xbias;
            y += ybias;
        }
        grid[(y*MAX_COLS)+x]++;
    }
}

int count_danger(char *grid) {
    int dangers = 0;
    for (int x = 0; x < GRID_SIZE; x++) {
        if (grid[x] > 1)
            dangers++;
    }

    return dangers;
}

void dump_grid(char *grid) {
    printf("   0123456789\n");
    printf("   ----------\n");
    for (int r = 0; r < 10; r++) {
        printf("%d| ", r);
        for (int c = 0; c < 10; c++) {
            char d = grid[r*MAX_ROWS+c];
            if (d > 0)
                printf("%d", d);
            else
                printf(".");
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    char *grid = calloc(sizeof(char), GRID_SIZE);

    if (args.run_first) {
        while (fgets(buf, sizeof(buf) - 1, args.input)) {
            vector v = parse_line(trim(buf));
            apply_vector_regular(v, grid);
        }

        // dump_grid(grid);

        int dangers = count_danger(grid);
        printf("first, number of dangerous cells is: %d\n", dangers);
    }

    if (args.run_second) {

        rewind(args.input);
        while (fgets(buf, sizeof(buf) - 1, args.input)) {
            vector v = parse_line(trim(buf));
            apply_vector_diagonal(v, grid);
        }

        // dump_grid(grid);

        int dangers = count_danger(grid);
        printf("second, number of dangerous cells with diagonals is: %d\n", dangers);
    }

    return 0;
}
