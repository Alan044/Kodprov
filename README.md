# Instructions

## In Windows

After signing in, search for "ThinLinc-klient" in the search box
in the lower left corner and open it. Enter the server
"thinlinc.student.it.uu.se", and your own username and password.
If you are asked whether you trust this host, click "Continue".
The new window you have is now running on the department's Linux
machines. You can make the window full-screen by clicking the
square in the top-right corner of the window. Avoid pressing the
"x" as this will close your connection!

## In Linux

Open a terminal and immediately write `mkdir kodprov240108`. Go to
this directory with `cd kodprov240108`. Download the code exam to
your computer with the following command:

    curl -L --remote-name http://eliasc.github.io/ioopm/schack.zip

You now have a zip file with the code for the exam. Unpack it with
`unzip schack.zip` (and enter the password given in the lab room)
and you will get *another* zip file called `kodprov.zip`.
Unpacking this with `unzip kodprov.zip` will get you a directory
`handout` containing the task to be solved, as well as a
`Makefile` used for handing in the code exam.

## Handing in

Go to the directory `kodprov240108`. If you are unsure about where
you are in the file system you can write `cd; cd kodprov240108`.
Write `make handin` to hand in the code exam. This will create a
zip file containing the file you are supposed to hand in (and no
other files), and this file will be saved in a location where we
can correct it.

## General rules

- The same rules as for a written exam applies: no mobile phones,
  no text messaging, no conversations with anyone except the
  course staff, regardless of medium.
- You must be able to identify yourself.
- You are allowed to bring a hand-written paper (maximum size A4).
  You can write on both sides but cannot bring a magnifying glass.
- You are allowed to bring a book, physically or on a tablet or
  laptop. You are not allowed to run any other programs on this
  machine, and you cannot use them for anything other than reading
  course literature.
- You must write all the code yourself, except the code that is
  given.

# Time Series

A *time series* is a sequence of values representing how some
value changes over time. These values could be measurement data
from a sensor, the price of a stock or bytes received over a wire.
The actual domain is not important here, but our values will be
non-negative integers (0 and above).

In addition to representing arbitrary time series, we are
interested in identifying particular patterns that we call
*waves*. A wave is a subsequence of a time series that is
non-decreasing. This means that values in a wave can remain the
same or increase, but never decrease. For example the series `1,
3, 3, 7, 2, 2, 4` can be seen as two waves: `1, 3, 3, 7` and `2, 2, 4`

## A Library for Time Series

The file `series.h` defines the interface for a library handling
time series. The type `series_t` represents a time series, i.e. a
sequence of integers of known length. Similar to how strings are
terminated by `\0`, a series will be terminated by the value `-1`.
The library supports the following operations:

- `series_create(int length, int source[], int offset)` -- create a
  time series containing `length` values from the array `source`,
  starting from index `offset` (see below for an example)
- `series_destroy(series_t *s)` -- destroy the series `s` and free
  any allocated memory
- `series_get(series_t *s, int i)` -- read the `i`th value of
  series `s`
- `series_length(series_t *s)` -- get the length of series `s`

The following example shows how to create a series given an array
as a source.

```c
int values[] = {3, 1, 4, 1, 5, 9};

// Create a subsequence of length 4, starting at index 1,
// i.e. the series 1, 4, 1, 5.
series_t *s = series_create(4, values, 1);

assert(series_length(s) == 4);
assert(series_get(s, 0) == 1);
assert(series_get(s, 1) == 4);
assert(series_get(s, 2) == 1);
assert(series_get(s, 3) == 5);

// The series is terminated by -1
assert(series_get(s, 4) == -1);

// Cleanup
series_destroy(s);
```

Finally, the function `series_next_wave(int source[], int *offset, int length)`
is used to create a series containing the (longest) wave from
`source`, starting from the index in the integer pointed to by
`offset` and not going further than `length` (i.e. `length` is the
number of elements in `source`). As a side-effect, the function
updates `offset` so that this memory stores the first index after
the wave. The `source` array will be terminated by `-1`, and this
value should never be included in the series.

The following example shows how to extract two waves from an array:

```c

int values[] = {1, 3, 3, 7, 2, 2, 4};
int offset = 0;
series_t *s1 = series_next_wave(values, &offset, 7);
// s1 now contains the series 1, 3, 3, 7
assert(series_length(s1) == 4);
// offset has been updated to the index after the first wave...
assert(offset == 4);

// ...which allows us to create a new series using the same variables
series_t *s2 = series_next_wave(values, &offset, 7);
// s2 now contains the series 2, 2, 4
assert(series_length(s2) == 3);
assert(offset == 7);

// Since we have reached the end of the array the next wave will be empty
series_t *s3 = series_next_wave(values, &offset, 7);
assert(series_length(s3) == 0);

// Cleanup omitted...
```

**Your task is to implement all the functions of `series.h` in the
file `series.c`. When you are done, you should be able to run the
tests through valgrind without any memory errors or leaks.**

## Files

- `series.h` -- The interface you should implement
- `series.c` -- You should write your implementation here
- `series_test.c` -- Tests for your library
- `Makefile`  -- A makefile that supports the following targets
  - `all`     -- Compile the program
  - `run`     -- Build and run the `main` function in `series.c`
  - `test`    -- Build and run the tests
  - `memrun` -- Build and run the `main` function in `series.c` with valgrind
  - `memtest` -- Build and run the tests with valgrind
  - `clean`   -- Remove any built files

Note that you can write tests of your own in the existing `main`
function of `series.c` and run it with `make run`. The code in the
`main` function will not be assessed.

## Hints

- Start by making your program *compile*. Write stubs for all the
  functions and have them return `NULL`, `0`, etc.
- If you want to use an array in your struct, remember that the
  type `int[]` in a struct has the special meaning "flexible array
  member". Although you *can* use that, you probably want to use
  `int *`.
- In order to create a series you need a length, a source and an
  offset into that source. When you are looking for the next wave,
  you already have the latter two of these, so you only need to
  count the number of values until one is smaller than the
  previous (or you reach the end of the source). Having the series
  length also lets you update the offset easily.
# Kodprov
