// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 3
// The one about reading binary numbers in a diagnostic report.
//
// Part 1 is to look at each bit position in all of the input numbers, determine
// if 0 or 1 is the most common, then use the most common value in the construction
// of a resulting number, the gamma rate. The epsilon rate is the same model
// except for the least common value; this really means it's the inversion of
// the gamma. Then the single output is to multiply the two values.
//
// Part 2 is to evaluate decreasingly small subsets of log values, defining the
// subset by choosing those with the matching value of the greatest count of
// values in successive positions. We're going to not create new subsets but
// just brute-force rescan the whole list, matching increasing numbers of bits
// from MSB to LSB.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define MAX_DIAGNOSTIC_LOGS     1500
#define MAX_DIAGNOSTIC_BITS     12

typedef struct log_t {
    int width;
    int counts[MAX_DIAGNOSTIC_BITS];
    int count;
    int *log;
} LOG;

int load_log(char *str, LOG *log) {
    int val = 0;

    for (int i = log->width - 1; i >= 0; i--) {
        if (*(str + i) - '0') {
            log->counts[i]++;
            val |= (1 << (log->width - i - 1));
        }
    }

    return val;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    LOG digits;
    memset(&digits, 0, sizeof(digits));
    digits.log = calloc(sizeof(int), MAX_DIAGNOSTIC_LOGS);

    fgets(buf, sizeof(buf) - 1, args.input);
    digits.width = strlen(trim(buf));

    do {
        digits.log[digits.count++] = load_log(trim(buf), &digits);
    } while (fgets(buf, sizeof(buf) - 1, args.input));

    if (args.run_first) {
        int gamma = 0, epsilon = 0;
        for (int i = 0; i < digits.width; i++) {
            int mask = (1 << (digits.width - i - 1));
            if (digits.counts[i] > (digits.count / 2)) {
                gamma |= mask;
            }
            else {
                epsilon |= mask;
            }
        }
        printf("first, gamma is %d and epsilon is %d, product is %d\n", gamma, epsilon, gamma * epsilon);
    }

    if (args.run_second) {
        int o2rating = 0;

        // MSB of rating is now set to the value of the most common value in MSB
        // of whole log dataset
        if (digits.counts[0] >= (digits.count / 2)) {
            o2rating = 1 << (digits.width - 1);
        }

        for (int d = digits.width - 1; d > 0; d--) {
            // the idea is that we have a match goal, like:
            //    match = 0b1010xxxxxxxx
            // and a mask like:
            //    mask = 0b111100000000
            // meaning we use the mask against the log value to hide bits we
            // don't care about, then use the match as an equality test to see
            // if it's a value we need to accumulate
            int mask = -1 << d;
            int eval_bit = 1 << (d - 1);
            int one_count = 0;
            int subset_count = 0;
            for (int l = 0; l < digits.count; l++) {
                if ((digits.log[l] & mask) == o2rating) {
                    subset_count++;
                    if (digits.log[l] & eval_bit)
                        one_count++;
                }
            }

            // ok if the count of one-bits is at least half the values, then
            // we add a 1-bit to the right spot in the match target
            if (one_count >= ((subset_count + 1) / 2)) {
                o2rating |= eval_bit;
            }
        }

        int co2rating = 0;

        // MSB of rating is now set to the value of the least common value in MSB
        // of whole log dataset
        if (digits.counts[0] < ((digits.count + 1) / 2)) {
            co2rating = 1 << (digits.width - 1);
        }

        for (int d = digits.width - 1; d > 0; d--) {
            int mask = -1 << d;
            int eval_bit = 1 << (d - 1);
            int one_count = 0;
            int subset_count = 0;
            int last_match = 0;

            for (int l = 0; l < digits.count; l++) {
                if ((digits.log[l] & mask) == co2rating) {
                    subset_count++;
                    last_match = digits.log[l];
                    if (digits.log[l] & eval_bit) {
                        one_count++;
                    }
                }
            }

            if (one_count < ((subset_count + 1)/ 2)) {
                co2rating |= eval_bit;
            }

            if (subset_count == 1) {
                co2rating = last_match;
                break;
            }
        }

        printf("second, O2 is %d and CO2 is %d, product is %d\n", o2rating, co2rating, o2rating * co2rating);
    }

    return 0;
}
