// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 8 - Seven Segment Search
// The one about malfunctioning 7-segment digital displays.
//
// So it looks like the first part is to observe that there are 4 digits that
// have a unique number of segments: 1 (2 seg), 4 (4 seg), 7 (3 seg), 8 (7 seg).
// The inputs have 10 patterns on the left of the '|' and 4 signal digits on the
// right. This part ignores the patterns and wants to count the number of these
// unique 1, 4, 7, 8 digits that appear among the signal digits on the right.
//
// Obvs, part 2 will involve something more complex using the junk on the left.
// It's clear that the pattern matching will come, and since the segments appear
// in different orders it will be useful to sort the segments within the pattern.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SEGMENTS    7
#define DIGITS      10
#define PATTERNS    10
#define SIGNALS     4

typedef struct {
    char digit;
    char segments[SEGMENTS + 1];
} digit_t;

typedef union {
    struct {
        digit_t *patterns[PATTERNS];
        digit_t *signals[SIGNALS];
    } d;
    digit_t *raw[PATTERNS + SIGNALS];
} noteentry_t;

digit_t digits[] = {
    { 0, "abcefg" },
    { 1, "cf" },
    { 2, "acdeg" },
    { 3, "acdfg" },
    { 4, "bcdf" },
    { 5, "abdfg" },
    { 6, "abdefg" },
    { 7, "acf" },
    { 8, "abcdefg" },
    { 9, "abcdfg" }
};

int compare_segments(const void *a, const void *b) {
    return (*(const char *)a - *(const char *)b);
}

digit_t *find_digit(char *pattern) {
    int l = strlen(pattern);
    if (l == 2)
        return digits + 1;
    else if (l == 3)
        return digits + 7;
    else if (l == 4)
        return digits + 4;
    else if (l == 7)
        return digits + 8;
    else {
        for (int i = 0; i < countof(digits); i++) {
            if (!strcmp(pattern, digits[i].segments))
                return digits + i;
        }
    }

    return NULL;
}

// noteentry_t parse_line(char *line) {
//     noteentry_t note = {};
//     char *p = line;
//     int ni = 0;

//     while (*p) {
//         char buf[20], *ch = buf;
//         while (isalpha(*p)) {
//             *ch = *p;
//             ch++;
//             p++;
//         }

//         while (isspace(*p) || *p == '|')
//             p++;
//         *ch = '\0';
//     }


//     return note;
// }

int count_known_digits(char *pattern) {
    int known = 0;
    char *p = pattern;

    while (*p && *p != '|')
        p++;
    p++;

    // printf("in pattern: %s", pattern);

    while (*p) {
        char buf[20], *ch = buf;
        while (isalpha(*p)) {
            *ch = *p;
            ch++;
            p++;
        }
        *ch = '\0';

        int l = ch - buf;
        // printf("looking for tokens of length %d\n", l);
        if (l == 2 || l == 3 || l == 4 || l == 7) {
            // printf(" found known %s\n", buf);
            known++;
        }

        while (*p && isspace(*p))
            p++;
    }

    return known;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    int known_digits = 0;
    while (fgets(buf, sizeof(buf) - 1, args.input)) {
        known_digits += count_known_digits(buf);
    }

    if (args.run_first) {
        printf("first, the number of unique known digits is: %d\n", known_digits);
    }

    if (args.run_second) {
    }

    return 0;
}