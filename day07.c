// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 7 - The Treachery of Whales
// The one about your sub, an attacking whale, and some swarming crab submarines.
//
// There's some contrivance of aligning the crabsubs on a horizontal position.
// I /think/ an insight is that the median of all the positions is the cheapest
// target to align them. So, capture all the positions (there are like 1000),
// sort and compute the median, and then sum the deltas between the median and
// each of the positions. This is the total fuel expended. I hope this holds
// up for the actual data set in addition to the sample.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SUBS    1000

int compare_crabs(const void *a, const void *b) {
    const short *na = (const short *)a;
    const short *nb = (const short *)b;

    if (*na < *nb)
        return -1;
    else if (*na == *nb)
        return 0;
    else
        return 1;
}

void dump_crabs(short *crabs, short crabcount) {
    for (int i = 0; i < crabcount; i++)
        printf("%d ", crabs[i]);

    printf("\n");
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);

    short crabcount = 0;
    short *crabs = calloc(MAX_SUBS, sizeof(short));
    int crabval;

    while (fscanf(args.input, "%d", &crabval) != EOF) {
        crabs[crabcount++] = crabval;
        fgetc(args.input);
    }

    qsort(crabs, crabcount, sizeof(short), compare_crabs);
    short median = crabs[crabcount / 2];

    if (args.run_first) {
        int fuel = 0;
        for (int i = 0; i < crabcount; i++) {
            fuel += (abs(crabs[i] - median));
        }
        printf("first, the sum of distances from the median %d is: %d\n", median, fuel);
    }

    if (args.run_second) {
        // printf("second, product of position %d and depth %d (with aim calculation) is: %d\n", position, depth, position * depth);
    }

    return 0;
}