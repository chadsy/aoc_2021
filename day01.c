// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 1
// The one about counting the number of increases in sea depth.
//
// Simple enough, just loop through all the depths andif the new one is greater
// than the previous.
//
// The second part is about doing the same thing, but comparing the sum of a
// sliding window of 3 depths instead of a single. Going to use the same loop
// model, except the window calc is for the current + previous two.

#include "aoc_common.h"
#include <stdlib.h>

#define MAX_DEPTHS  3000

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    int depth_count = 0;
    int *depths = calloc(sizeof(int), MAX_DEPTHS);

    while (fgets(buf, sizeof(buf) - 1, args.input)) {
        depths[depth_count] = atoi(buf);
        depth_count++;
        if (depth_count > MAX_DEPTHS) {
            fprintf(stderr, "error: %d input depths exceeded max\n", depth_count);
            break;
        }
    }

    if (args.run_first) {
        int increases = 0;
        int last_depth = depths[0];
        for (int d = 1; d < depth_count; d++) {
            if (depths[d] > last_depth) {
                increases++;
            }
            last_depth = depths[d];
        }

        printf("first, number of increases in depths: %d\n", increases);
    }

    if (args.run_second) {
        int increases = 0;
        int last_window = depths[0] + depths[1] + depths[2];

        for (int d = 3; d < depth_count; d++) {
            int window = depths[d - 2] + depths[d - 1] + depths[d];
            if (window > last_window) {
                increases++;
            }
            last_window = window;
        }

        printf("second, number of increases in sliding window of 3 depths: %d\n", increases);
    }

    return 0;
}