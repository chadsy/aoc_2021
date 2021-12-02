// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Day 2
// The one about following up/down/forward instructions to move the sub.
//
// Parsing the input is just a sscanf of %s %d, where each instruction is one of
// 'forward', 'down', and 'up', followed by an integer. Should just be simple
// add and subtract on two values, position and depth. The puzzle unlocks by
// multiplying the two values to get a single number.
//
// Part 2 is just to add a bit more complexity to the basic process, tracking an
// 'aim' in addition to position and depth, then on 'forward' use a different
// calculation: increase position (as before) then increase depth by the current
// aim mutiplied by the forward instruction amount.

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS    1500

typedef enum {
    forward,
    up,
    down
} COMMAND;

typedef struct instruction_t {
    COMMAND cmd;
    int     amt;
} instruction;

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);
    char buf[128];

    int inst_count = 0;
    instruction *instructions = calloc(sizeof(int), MAX_INSTRUCTIONS);

    while (fgets(buf, sizeof(buf) - 1, args.input)) {
        instruction inst;
        char cmd[20];

        if (sscanf(buf, "%s %d", cmd, &inst.amt) != 2) {
            fprintf(stderr, "error: cannot parse input instruction '%s'\n", buf);
        }

        if (strcmp(cmd, "forward") == 0)
            inst.cmd = forward;
        else if (strcmp(cmd, "up") == 0)
            inst.cmd = up;
        else if (strcmp(cmd, "down") == 0)
            inst.cmd = down;
        else
            fprintf(stderr, "error: cannot parse input instruction '%s'\n", buf);

        instructions[inst_count] = inst;
        inst_count++;
    }

    if (args.run_first) {
        int position = 0;
        int depth = 0;
        for (int i = 0; i < inst_count; i++) {
            instruction cmd = instructions[i];
            switch (cmd.cmd) {
            case forward:
                position += cmd.amt;
                break;
            case up:
                depth -= cmd.amt;
                break;
            case down:
                depth += cmd.amt;
                break;
            }
        }

        printf("first, product of position %d and depth %d is: %d\n", position, depth, position * depth);
    }

    if (args.run_second) {
        int position = 0;
        int depth = 0;
        int aim = 0;
        for (int i = 0; i < inst_count; i++) {
            instruction cmd = instructions[i];
            switch (cmd.cmd) {
            case forward:
                position += cmd.amt;
                depth += aim * cmd.amt;
                break;
            case up:
                aim -= cmd.amt;
                break;
            case down:
                aim += cmd.amt;
                break;
            }
        }

        printf("second, product of position %d and depth %d (with aim calculation) is: %d\n", position, depth, position * depth);
    }

    return 0;
}