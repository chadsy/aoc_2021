// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 4
// The one about bingo with a squid.
//
// First thing is, read in a comma-separated list of ints that are the bingo
// draw numbers. Then read a series of 5x5 arrays of bingo boards. Then 'play'
// the bingo draw numbers on all the boards. After each one, look for a row or
// column of 'found' numbers on the boards. In C, there are no strong data
// structures for this, so, there's a question of how to store the boards and
// their 'found' status bits for each of the spots.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define BOARD_COLS  5
#define BOARD_ROWS  5
#define BOARD_SIZE  (BOARD_ROWS * BOARD_COLS)
#define MAX_BOARDS  100
#define MAX_BALLS   128

typedef struct {
    short *cells;
    bool *found;
    bool winner;
} board;

board create_board() {
    board b;
    b.cells = calloc(BOARD_SIZE, sizeof(short));
    b.found = calloc(BOARD_SIZE, sizeof(bool));
    b.winner = false;
    return b;
}

board load_board(FILE *f) {
    board b = create_board();

    for (int i = 0; i < BOARD_ROWS; i++) {
        short *num = b.cells + (i * BOARD_COLS);
        fscanf(f, "%hd %hd %hd %hd %hd\n", num, num + 1, num + 2, num + 3, num + 4);
    }

    return b;
}

bool check_winner(board b) {
    // check for winning rows
    for (int r = 0; r < BOARD_ROWS; r++) {
        bool *founds = b.found + (r * BOARD_COLS);
        if (founds[0] && founds[1] && founds[2] && founds[3] && founds[4])
            return true;
    }

    for (int c = 0; c < BOARD_COLS; c++) {
        bool *founds = b.found + (c * BOARD_ROWS);
        if (founds[0] && founds[1 * BOARD_COLS] && founds[2 * BOARD_COLS] && founds[3 * BOARD_COLS] && founds[4 * BOARD_COLS])
            return true;
    }

    return false;
}

bool apply_ball(board *b, int ball) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (b->cells[i] == ball) {
            b->found[i] = true;
            b->winner = check_winner(*b);
            break;
        }
    }
    return b->winner;
}

void skip_newlines(FILE *f) {
    int c = 0;
    do {
        c = fgetc(f);
    } while (c == '\n' && c != EOF);
    ungetc(c, f);
}

int sum_unmarked(board b) {
    int sum = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (!b.found[i])
            sum += b.cells[i];
    }
    return sum;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[400];

    short *balls = calloc(MAX_BALLS, sizeof(short));
    int ball_count = 0;

    // Load the list of called balls, one long string
    fgets(buf, sizeof(buf) - 1, args.input);
    char *p = buf;
    while (*p) {
        balls[ball_count++] = atoi(p);
        while (isdigit(*p))
            p++;
        while (*p && !isdigit(*p))
            p++;
    }

    board *boards = calloc(sizeof(board), MAX_BOARDS);
    int board_count = 0;

    // Now load the boards as groups of BOARD_ROWS, with newlines in between
    while (!feof(args.input)) {
        skip_newlines(args.input);
        if (feof(args.input))
            break;
        board b = load_board(args.input);
        boards[board_count++] = b;
    }

    if (args.run_first) {
        for (int i = 0; i < ball_count; i++) {
            for (int b = 0; b < board_count; b++) {
                apply_ball(boards + b, balls[i]);
                if (boards[b].winner) {
                    int sum = sum_unmarked(boards[b]);
                    printf("first, winner in board %d: last number %d, sum of unmarked numbers %d: product is %d\n",
                        b, balls[i], sum, sum * balls[i]);
                    // force breaking out of the outer loop
                    i = ball_count;
                    break;
                }
            }
        }
    }

    if (args.run_second) {
        // printf("second, product of position %d and depth %d (with aim calculation) is: %d\n", position, depth, position * depth);
    }

    return 0;
}