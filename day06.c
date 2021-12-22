// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 6 - Lanternfish
// The one about the spawning habits of a specific school of lanternfish.
//
// This is like loading a big array of 'fish', each of which has a reproduction
// rate of 8, but is at a different place on their gestation cycle. So tick time
// forward, and if the gestation period expires, create a new fish.
//
// I can't help but think there's a solution that merely captures how many fish
// are on each possible day of the cycle, does some multiply and divide and
// modulus to figure out how many new ones are spawned...but then, it still sort
// of requires phases for future-born fish. Then again, maybe just start with a
// short array, of size gestation-period, where each element is the number of
// fish alive on that day of the period. Then a 'day' is just to spawn new ones
// on day 8, add the day-0/spawner count to day 6, and rotate everyone forward.
//
// Part two is...

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define GESTATION_PERIOD    9
#define FISH_FORMAT         "%lld"
typedef long long fishcount;
typedef long long fishsum;

fishsum sum_all_fishes(fishcount *periods) {
    fishsum sum = 0;

    for (int i = 0; i < GESTATION_PERIOD; i++) {
        sum += periods[i];
    }

    return sum;
}

void rotate_periods(fishcount *periods) {
    fishcount zero = *periods;
    memcpy(periods, periods + 1, sizeof(fishcount) * (GESTATION_PERIOD - 1));
    periods[GESTATION_PERIOD - 3] += zero;
    periods[GESTATION_PERIOD - 1] = zero;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);

    fishcount periods[GESTATION_PERIOD + 1] = {};
    int ch;

    // the cycles are all single-digit so the parsing is pretty easy
    while ((ch = fgetc(args.input)) != EOF) {
        if (isdigit(ch)) {
            int slot = ch - '0';
            periods[slot]++;
        }
    }

    if (args.run_first) {
        fishcount fishes[GESTATION_PERIOD + 1];
        memcpy(fishes, periods, sizeof(fishes));
        int days = 80;
        for (int day = 0; day < days; day++) {
            rotate_periods(fishes);
        }

        fishsum sum = sum_all_fishes(fishes);
        printf("first, sum of all fishes after %d days: " FISH_FORMAT "\n", days, sum);
    }

    if (args.run_second) {
        fishcount fishes[GESTATION_PERIOD + 1];
        memcpy(fishes, periods, sizeof(fishes));
        int days = 256;
        for (int day = 0; day < days; day++) {
            rotate_periods(fishes);
        }

        fishsum sum = sum_all_fishes(fishes);
        printf("second, sum of all fishes after %d days: " FISH_FORMAT "\n", days, sum);
    }

    return 0;
}