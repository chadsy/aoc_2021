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
//
// Turns out, median worked but I'm not sure that's mathmatically sustainable
// for any data set. I feel like there's another function (not mean, not median,
// not mode) that captures the top of the bell curve of the distribution.
//
// Part 2 is that crabs' subs don't burn fuel at a constant rate, so it's 1 unit
// for 1st step, 2 units for 2nd, 3 units for 3rd. This can be expressed as:
// n * ((n + 1)/2) for the calculation. Fine, but is the median still the best
// solution? Maybe it's now the mean? That works for the sample set;
//
// It does not work for the full set, 90041060 is too high for the mean of 465.
// So I started with the mean value, sampled mean + 1, and decided to just
// brute-force search up or down from there. This worked out fine, mean - 1 is
// the right distance, the answer is 90040997.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

int score_exponential_fuel(short *crabs, short crabcount, int target) {
    int fuel = 0;

    for (int i = 0; i < crabcount; i++) {
        int distance = abs(crabs[i] - target);
        float factor = (distance + 1) / 2.0f;
        int fuel_calc = (int)lroundf(distance * factor);
        fuel += fuel_calc;
    }

    return fuel;
}

int score_linear_fuel(short *crabs, short crabcount, int target) {
    int fuel = 0;

    for (int i = 0; i < crabcount; i++) {
        fuel += (abs(crabs[i] - target));
    }
    return fuel;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);

    short crabcount = 0;
    short *crabs = calloc(MAX_SUBS, sizeof(*crabs));

    int crabval;
    int crabsum = 0;
    while (fscanf(args.input, "%d", &crabval) != EOF) {
        crabs[crabcount++] = crabval;
        crabsum += crabval;
        // eat the comma
        fgetc(args.input);
    }

    short mean = (short)lroundf((float)crabsum / crabcount);

    qsort(crabs, crabcount, sizeof(*crabs), compare_crabs);
    short median = crabs[crabcount / 2];

    if (args.run_first) {
        int fuel = score_linear_fuel(crabs, crabcount, median);
        printf("first, the sum of fuel from the median %d is: %d\n", median, fuel);
    }

    if (args.run_second) {
        int meanfuel = score_exponential_fuel(crabs, crabcount, mean);
        int plusfuel = score_exponential_fuel(crabs, crabcount, mean + 1);
        int lowfuel = plusfuel;
        int bias = 1;
        int distance = mean + 1;

        if (meanfuel < plusfuel) {      // going down...
            bias = -1;
            lowfuel = meanfuel;
            distance = mean - 1;
        }

        int lastfuel = 0;
        while(distance >= 0 && distance <= crabs[crabcount - 1]) {
            lastfuel = score_exponential_fuel(crabs, crabcount, distance);
            if (lastfuel <= lowfuel) {
                lowfuel = lastfuel;
                break;
            }
            distance += bias;
        }

        printf("second, the sum of fuel with resistance from the mean %d is: %d\n", distance, lowfuel);
    }

    return 0;
}