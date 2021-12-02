# aoc_2021
Solving the Advent of Code puzzles in 2021 using simple C, maybe, as a kind of code kata, maybe.


# Advent of Code 2021: Code Kata in C

I'm solving the [2021 Advent of Code](https://adventofcode.com/2021) puzzles using simple C, as a kind of daily code kata. I intend to stick with bog-standard ANSI-C89 for the most part, except for the single-line comment style. So maybe we call it C89-ish, but then there are also some later libc and 3rd party things that I've used. I have no idea what kind of patterns and idioms will creep in, as my head's version of C programming is a mix of like 20 different languages since I first learned and used pre-ANSI C.

Again: Grateful for these annual puzzles that [Eric Wastl](http://was.tl/) puts together.

I'm using a simple editor and terminal, with a simple makefile. And I'm trying to not rely on code navigation features or statement completion or a debugger or a profiler.

# Project Layout

The project root folder contains a series of `dayXX.c` files, each source file contains solutions for both the part 1 and part 2 challenges in the puzzle for that day. There's an `aoc_common.h` file that has a few helper things, nothing major that affects the algorithms or data structures of the solutions. The `./inputs` folder contains the input data provided for each puzzle, each one used for both parts 1 and 2. The `./build` folder contains the compiled programs as `.app` files and their dSYM folders. So running one looks something like:

```bash
> build/day02.app inputs/day02.txt
total paper: 12345
total ribbon: 67890

# run only the 1st part
> build/day02.app 1 inputs/day02.txt
total paper: 12345

# run only the 2nd part
> build/day02.app 2 inputs/day02.txt
total ribbon: 67890

# run using stdin as the input, interactively or with input pipe
> build/day02.app
> 200
> 300
> 400
> ^D
total paper: 12345
total ribbon: 67890
```

# Makefile

* `dayXX` will build and run that specific day's app. This is the sweet spot for testing, just `make day02`, etc.
* `clean` just what it says, which is to clear out the /build folder where the resulting apps are stored

# Dependencies

* `clang` is used in the makefile, which for xcode users like me maps to `clang v12.0.5`. I didn't test with an actual gcc or other versions, etc.
* `make` is used, but should be super-basic. If it's not fully portable, it's bog-simple to understand and replicate.

# License

All this is under [MIT License](http://opensource.org/licenses/MIT). Share and enjoy.
