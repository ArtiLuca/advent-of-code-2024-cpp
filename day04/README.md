# Advent of Code 2024 - Day 4: Ceres Search

## Part 1

In Part 1, we are given a word search as a 2D grid of characters and are asked to search for the word `XMAS`.

The word search allows words to be horizontal, vertical, diagonal, written backwards, or even overlapping other words.

We want to count the total number of occurrences of `XMAS` in the word search for Part 1's solution.

## Idea

To solve this efficiently, I used two common tricks for grid problems:

### 1. Direction Arrays

For any given cell `(r, c)` containing an `X`, we must check the 8 adjacent directions for the remaining `M`, `A`, and `S` characters.

The 8 adjacent positions are:

```text
(r-1,c-1)   (r-1,c)   (r-1,c+1)
     ↖         ↑         ↗
(r,c-1)  ←   (r,c)   →  (r,c+1)
     ↙         ↓         ↘
(r+1,c-1)   (r+1,c)   (r+1,c+1)
```

Instead of writing 8 separate checks manually, we can scan all possible directions by using directional arrays, which represent the row (`dr`) and column (`dc`) step modifications:

```text
dr = {-1, -1, -1,  0,  0,  1,  1,  1}
dc = {-1,  0,  1, -1,  1, -1,  0,  1}
```

Then, for any cell `(r, c)`, the `k`-th direction is checked by evaluating `(r + dr[k], c + dc[k])`.

### 2. Padding

To avoid out-of-bound errors and boundary checks, we can add extra padding to the 2D grid.

Since we are always checking exactly three steps outwards from an `X` for the remaining `MAS` characters, we can add three extra layers of padding, using `.`, to each side of our grid.

If the original grid has dimensions `R × C`, the stored grid expands to:

```text
(R + 6) × (C + 6)
```

The effective grid area on which we operate is located between rows `3` and `R + 2`, and columns `3` and `C + 2`.

This lets us safely check all 8 directions 3 steps deep without ever worrying about going out of bounds.

## Pseudocode

Assuming `G` is our padded 2D grid, `target` is the string `"XMAS"`, and `dr` and `dc` contain our directional arrays:

```text
solvePart1(G, target)
    total = 0

    // assuming R and C are the original puzzle dimensions
    for r = 3 to R + 2
        for c = 3 to C + 2

            // we only check directions if we start on an 'X'
            if G[r,c] == 'X'
                for d = 0 to 7
                    found = true

                    // we already know i = 0 is 'X',
                    // so we only check 1, 2, 3 ('M', 'A', 'S')
                    for i = 1 to 3
                        if G[r + i * dr[d], c + i * dc[d]] != target[i]
                            found = false
                            break

                    if found == true
                        total = total + 1

    return total
```

## Complexity

The **Time Complexity** is `O(R * C)`, where `R` is the number of rows and `C` is the number of columns in the original puzzle grid.

We iterate through the valid searchable area of the grid exactly once. For each cell containing an `X`, we perform a constant amount of work, checking at most 8 directions and 3 subsequent characters.

This inner work is strictly bounded, resulting in a time complexity that is linear relative to the total number of cells.

The **Space Complexity** is `O(R * C)`.

We store the entire puzzle grid in memory along with three extra layers of padding per side, yielding:

```text
O((R + 6) * (C + 6))
```

which simplifies to:

```text
O(R * C)
```

---

## Part 2

In Part 2 we are told that the word search is actually an _X-MAS_ puzzle.

Meaning, we need to find two `MAS` in the shape of an `X`, for example like shown below, where irrelevant characters are shown as `.`:

```text
M . S
. A .
M . S
```

Two `MAS` written as an `X` form a single `X-MAS`, where the two `MAS` that form it can be written either forwards or backwards.

We are asked to count the total number of `X-MAS` found in the word search for Part 2's solution.

## Idea

We can take advantage of the padding used in Part 1.

In particular, for Part 2 we only need to check one step outwards from each `A`, so one layer of padding would be enough. However, since my input-reading function already applies three layers of padding for Part 1, we can safely reuse the same padded grid.

For each cell where `G[r,c] == 'A'`, we must check the two diagonals that form the `X`.

Specifically, we check:

- the diagonal formed by the top-left character `(r-1,c-1)` and the bottom-right character `(r+1,c+1)`;
- the diagonal formed by the top-right character `(r-1,c+1)` and the bottom-left character `(r+1,c-1)`.

In both cases, the two characters on the diagonal must be one `M` and one `S`.

This means each diagonal can be either `MAS` or `SAM`.

## Pseudocode

Assuming `G` is our padded 2D grid:

```text
solvePart2(G)
    total = 0

    // assuming R and C are the original puzzle dimensions
    for r = 3 to R + 2
        for c = 3 to C + 2

            // whenever we encounter a possible X-MAS center
            if G[r,c] == 'A'

                // check top-left to bottom-right diagonal
                top_left = G[r-1,c-1]
                bottom_right = G[r+1,c+1]

                // check top-right to bottom-left diagonal
                top_right = G[r-1,c+1]
                bottom_left = G[r+1,c-1]

                diag_one = ((top_left == 'M') and (bottom_right == 'S')) or
                           ((top_left == 'S') and (bottom_right == 'M'))

                diag_two = ((top_right == 'M') and (bottom_left == 'S')) or
                           ((top_right == 'S') and (bottom_left == 'M'))

                if diag_one == true and diag_two == true
                    total = total + 1

    return total
```

## Complexity

The **Time Complexity** remains `O(R * C)`.

We scan the effective grid once, and for each cell containing an `A`, we check only four diagonal characters.

The **Space Complexity** also remains `O(R * C)`, because we store the padded grid in memory.

---

## Build

```bash
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp day04.cpp -o day04
```

## Run

```bash
./day04
```

The program expects the puzzle input in a file named `input.txt`.

---

## Files

- `main.cpp` — creates the `WordSearch` solver, reads the input file, and prints both answers
- `day04.h` — declares the `WordSearch` class, the direction arrays, and the target string
- `day04.cpp` — handles input parsing, grid padding, the Part 1 `XMAS` search, and the Part 2 `X-MAS` diagonal check