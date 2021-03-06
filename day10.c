// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License https://opensource.org/licenses/MIT
//
// Day 10 - Syntax Scoring
// The one about reading a bunch of matched pair characters.
//
// It's all about matching [], (), <>, and {}. A 'corrupt' line is one where
// one of the enclosures closes with the wrong character. IOW, if a closing
// character (e.g. ']}>)') is found but it's not the one expected, then that's
// corrupt. If something's incomplete, it means the line ends (we run out of
// syntax) before we close everything up. This part is about the corrupt lines.
//
// Then, for a corrupt line and score the illegal character:
// ) - 3 pts
// ] - 57 pts
// } - 1197 pts
// > - 25137 pts
// then sum up all the point values for the illegal characters. I could create
// a parser-like state machine, or I could just use recursion. Recursion it is.
//
// Part 2 is to discard the 'corrupt' lines and focus on the incomplete lines.
// "You just need to figure out the sequence of closing characters". This is
// basically 'autocomplete' for the pairs. So with each collection of characters
// in the correct order to complete the line, do some gnarly scorekeeping. Then,
// bog help us, we have to collect, sort, and find the median.
//
// It seems that I still need the same seek function, but modified that it also
// needs to create a stack of sought characters. The remaining sought characters
// is what should be scored. So maybe a simple stack of those?
//
// So here near the end of Part 2, I have a solution that works with the sample
// data for both Part 1 and Part 2. Sadly, it fails for both parts in the real
// data :( In Part 1, it gets one too many ']' for an extra 57 points. And then
// 1048024721 is too low for Part 2. Ugh. So I folded back to an earlier set of
// code and fixed part 1. Then tackled part 2 after I sorted out some crap
// around the size of the integer needed to carry the score, fixed the sort
// and median logic, etc.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINES   100
#define SEEK_STACK  20

char seek_stack[SEEK_STACK];
int seek_count = 0;

void reset_seek() {
    memset(seek_stack, 0, sizeof(seek_stack));
    seek_count = 0;
}

void push_seek(char c) {
    seek_stack[seek_count++] = c;
}

char pop_seek() {
    char c = 0;
    if (seek_count > 0) {
        c = seek_stack[--seek_count];
        seek_stack[seek_count] = 0;
    }
    else
        fprintf(stderr, "error: stack underflow in seek stack\n");

    return c;
}

char *get_seek_stack() {
    static char rev[SEEK_STACK + 1];
    memset(rev, 0, sizeof(rev));

    char *s = seek_stack + seek_count - 1;
    char *r = rev;

    while (s >= seek_stack)
        *r++ = *s--;

    return rev;
}

long score_seek_stack() {
    long score = 0;

    char *completion = get_seek_stack();

    char *specials = ")]}>";
    while (*completion) {
        score *= 5;
        score += (strchr(specials, *completion) - specials) + 1;
        completion++;
    }

    return score;
}

char seek_closure(char **line, char seek) {
    char illegal = 0;

    if (seek) {
        push_seek(seek);
    }

    while (**line && illegal == 0) {
        if (**line == seek) {
            pop_seek();
            return 0;
        }

        switch(**line) {
        case '(':
            ++(*line);
            illegal = seek_closure(line, ')');
            break;
        case '[':
            ++(*line);
            illegal = seek_closure(line, ']');
            break;
        case '{':
            ++(*line);
            illegal = seek_closure(line, '}');
            break;
        case '<':
            ++(*line);
            illegal = seek_closure(line, '>');
            break;
        default:
            illegal = **line;
            return illegal;
        }

        if (**line) {
            (*line)++;
        }
    }

    return illegal;
}

int compare_scores(const void *a, const void *b) {
    long ax = *(const long *)a;
    long bx = *(const long *)b;

    if (ax < bx)
        return -1;
    else if (ax == bx)
        return 0;
    else
        return 1;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);

    char *lines[MAX_LINES];
    int line_count = 0;

    char buf[128];
    while (fgets(buf, sizeof(buf) - 1, args.input)) {
        lines[line_count++] = strdup(trim(buf));
    }

    if (args.run_first) {
        int score = 0;

        for (int i = 0; i < line_count; i++) {
            char *base_line = lines[i];

            char unexpected = seek_closure(&base_line, 0);

            switch (unexpected) {
            case ')':
                score += 3;
                break;
            case ']':
                score += 57;
                break;
            case '}':
                score += 1197;
                break;
            case '>':
                score += 25137;
                break;
            }
        }

        printf("first, the sum of corruption scores is: %d\n", score);
    }

    if (args.run_second) {
        long scores[MAX_LINES] = {};
        int score_count = 0;

        for (int i = 0; i < line_count; i++) {
            char *base_line = lines[i];

            reset_seek();
            char unexpected = seek_closure(&base_line, 0);

            // we've scored it an accumulated any interesting remaining seeks
            // but we only want to deal with it if the score is 0, meaning, it's
            // not corrupt.
            if (unexpected == 0) {
                scores[score_count++] = score_seek_stack();
            }
        }

        // sort the scores array
        qsort(scores, score_count, sizeof(long), compare_scores);

        // find the median of the array
        long median = scores[score_count / 2];

        printf("second, the median score of completion strings is: %ld\n", median);
    }

    return 0;
}
