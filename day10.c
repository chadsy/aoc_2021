// AOC 2021 solution in C
// @chadsy
// Copyright (C) 2021 Chad Royal
// MIT License http://opensource.org/licenses/MIT
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

#include "aoc_common.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LINES   100

char seek_closure(char **line, char seek) {
    char illegal = 0;

    // static int level = 0;
    // level++;
    // printf("from '%s' seeking '%c' at %d\n", *line, seek, level);

    while (**line && illegal == 0) {
        if (**line == seek) {
            // printf("found expected '%c' at %d\n", seek, level);
            // level--;
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
            // printf("returning unexpected '%c' from level %d, sought %c\n", illegal, level, seek);
            // level--;
            return illegal;
        }

        if (**line)
            (*line)++;
    }

    // if (illegal)
    //     printf("(tunnel from level %d)\n", level);
    // level--;

    return illegal;
}

int score_corrupt_line(char *line) {
    int score = 0;
    char illegal = 0;

    // printf("with '%s'\n", line);

    if (*line == '(') {
        line++;
        illegal = seek_closure(&line, ')');
    }
    else if (*line == '[') {
        line++;
        illegal = seek_closure(&line, ']');
    }
    else if (*line == '{') {
        line++;
        illegal = seek_closure(&line, '}');
    }
    else if (*line == '<') {
        line++;
        illegal = seek_closure(&line, '>');
    }

    switch (illegal) {
    case ')':
        score = 3;
        break;
    case ']':
        score = 57;
        break;
    case '}':
        score = 1197;
        break;
    case '>':
        score = 25137;
        break;
    }

    // if (score)
    //     printf("illegal character '%c': %d\n", illegal, score);
    // else
    //     printf("no error\n");

    return score;
}

int main(int argc, char **argv) {
    runargs args = parse_args(argc, argv);

    char *lines[MAX_LINES];
    int line_count = 0;

    char buf[128];
    while (fgets(buf, sizeof(buf) - 1, args.input)) {
        lines[line_count++] = strdup(trim(buf));
    }

    // lines[line_count++] = "[({(<(())[]>[[{[]{<()<>>";   // 0, good
    // lines[line_count++] = "[(()[<>])]({[<{<<[]>>(";     // 1, good
    // lines[line_count++] = "{([(<{}[<>[]}>{[]{[(<()>";   // 2, bad, found }
    // lines[line_count++] = "[[<[([]))<([[{}[[()]]]";  // 4, bad, found )
    // lines[line_count++] = "[{[{({}]{}}([{[{{{}}([]"; // 5, bad, found ]
    // lines[line_count++] = "{{[]()<>}}";

    if (args.run_first) {
        int score = 0;

        for (int i = 0; i < line_count; i++) {
            score += score_corrupt_line(lines[i]);
        }

        printf("first, the sum of corruption scores is: %d\n", score);
    }

    if (args.run_second) {
        // printf("second, product of position %d and depth %d (with aim calculation) is: %d\n", position, depth, position * depth);
    }

    return 0;
}
