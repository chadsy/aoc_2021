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
//
// So, yeah, Part 2. Every line represents a different encoding. So the trick is
// to deduce the encoding based on using length to identify the 1, 4, 7, 8
// segments. So for example, finding a len 3 (7) and comparing to it the segments
// from any found len 2 (1) will tell you what segment label is the top horz
// segment. I guess there's a whole set of rules that I need think about and
// write code to implement.
//
// Additionally, I have the question of whether it's interesting at all to try
// to be smart and parse all this shit and hydrate date structures. I suspect
// no. So guess I'll simplify and focus the solution to part 2 on deducing each
// line's thing. Aside from the deduction logic, there's nothing to store for
// any analysis.
//
// Further, it seems even simpler. I don't have to decode these or even try to
// understand which segments map to which letter. I can just see how many
// segment labels, whatever they are for a given line, are common with known
// values, for digits of the same length. 1 and 8 are useless, 4 & 7 are key.
//
// So there's a truth table that looks like this:
//        common with
// 7-seg    4     7      length
//  0       3     3         6
//  1       -     -         2
//  2       2     2         5
//  3       3     2         5
//  4       -     -         4
//  5       3     2         5
//  6       3     2         6
//  7       -     -         3
//  8       -     -         7
//  9       4     3         6
//
// At the end of this, I wrote like 4 different versions of these two parts,
// each one simpler and more 'insightful' and 'constrained' than the last.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SEGMENTS    7
#define SIGNALS     4

int count_shared_segments(char *signal, char *reference) {
    int shared = 0;

    while (*signal) {
        if (strchr(reference, *signal))
            shared++;
        signal++;
    }

    return shared;
}

int infer_digit(char *sig, char *four, char *seven) {
    int l = strlen(sig);
    int shared4, shared7;

    switch (l) {
    case 2:
        return 1;
    case 3:
        return 7;
    case 4:
        return 4;
    case 7:
        return 8;
    case 5:
    case 6:
        shared4 = count_shared_segments(sig, four);
        shared7 = count_shared_segments(sig, seven);
        if (l == 5) {
            // figure out 2, 3, 5 based on shared segments
            if (shared7 == 3)
                return 3;
            else if (shared4 == 2)
                return 2;
            else
                return 5;
        }
        else {
            // figure out 6, 9, 0 based on shared segments
            if (shared7 == 2)
                return 6;
            else if (shared4 == 4)
                return 9;
            else
                return 0;
        }
    }
    return -1;
}

void find_four_seven(char *line, char *four, char *seven) {
    char *org = line;
    // find an example of 4 & 3 segments
    while (*line && (!*four || !*seven)) {
        char tmp[SEGMENTS + 1] = {};

        while (isspace(*line))
            line++;

        if (sscanf(line, "%s", tmp) == 1) {
            if (strlen(tmp) == 3)
                strcpy(seven, tmp);
            else if (strlen(tmp) == 4)
                strcpy(four, tmp);

            line += strlen(tmp);
        }
        else {
            printf("error: unable to parse a pattern at '%-.10s'\n       ...from line %s\n", line, org);
            break;
        }
    }
}

int count_known_digits(char *line) {
    while (*line && *line != '|')
        line++;
    while (!isalpha(*line))
        line++;

    int known = 0;
    while (*line) {
        char *start = line;
        while (isalpha(*line))
            line++;

        int l = line - start;
        if (l == 2 || l == 3 || l == 4 || l == 7) {
            known++;
        }

        while (*line && !isalpha(*line))
            line++;
    }
    return known;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    if (args.run_first) {
        int known_digits = 0;
        while (fgets(buf, sizeof(buf) - 1, args.input)) {
            known_digits += count_known_digits(trim(buf));
        }

        printf("first, the number of unique known digits is: %d\n", known_digits);
    }

    if (args.run_second) {
        rewind(args.input);

        long sum = 0;
        while (fgets(buf, sizeof(buf) - 1, args.input)) {
            trim(buf);
            char four[SEGMENTS + 1] = {};
            char seven[SEGMENTS + 1] = {};

            find_four_seven(buf, four, seven);

            // Skip past the pattern | signal delimeter
            char *line = buf;
            while (*line && *line != '|')
                line++;
            while (!isalpha(*line))
                line++;

            char signal[SIGNALS + 1] = {};
            char *dig = signal;
            while (*line) {
                char tmp[SEGMENTS + 1];
                while (isspace(*line))
                    line++;
                if (sscanf(line, "%s", tmp) == 1) {
                    int d = infer_digit(tmp, four, seven);
                    *dig = '0' + d;
                    dig++;
                }

                line += strlen(tmp);
            }

            sum += atoi(signal);

        }
        printf("second, the sum of signal digits is: %ld\n", sum);
    }

    return 0;
}
