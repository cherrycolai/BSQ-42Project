# BSQ - 42 Piscine Final Project

This project's aim is to locate the biggest empty square inside a 2D map of obstacles and fill it with a marker character.

## Build

```sh
make
```

The code compiles with `cc -Wall -Wextra -Werror`. The build is incremental and the binary will not relink unnecessarily.

## Usage

You can pass multiple map files as arguments or read a single map directly from standard input.

```sh
./bsq map_file [map_file ...]   # Process one or more map files
./bsq < map_file                # or read a single map from stdin
```


## Map format Specification

A valid map consists of a header line followed by the grid itself.

* Header: <number_of_rows><empty_char><obstacle_char><full_char>\n

* * The last three characters before the newline define the map's alphabet.

* * They must be printable, strictly pairwise distinct, and can include spaces or digits.

* Grid: Every subsequent line must contain only the defined empty or obstacle characters, and all rows must share the exact same width.

## Approach

* The file is slurped into a single buffer with a chunked-read loop (norealloc).
* The header is parsed by locating the first newline and stripping the three special characters from its tail.
* The grid is allocated row by row and the input is copied while every cell is validated against the declared alphabet.
* The largest square is computed with a classic O(rows * cols) dynamic programming pass stored in a single flat `unsigned int` array indexed as `dp[i * cols + j]`.
* The bottom-right corner of the max value is tracked during the same pass; iteration order naturally yields the topmost-then-leftmost square required by the subject.

## Codebase Layout

```
includes/bsq.h     Core types, macros, and function prototypes
srcs/main.c        Entry point and argument dispatching
srcs/io.c          Chunked reader and standardized write helpers
srcs/parse.c       Header extraction and strict cell validation
srcs/grid.c        Canvas memory management (allocation/free) and printing
srcs/solver.c      Core DP algorithm and max-square coordinate search
srcs/runner.c      Per-file execution pipeline and square stamping
```