# Advent of Code 2024 - Day 2: Red-Nosed Reports

## Part 1

In Part 1 we are given a list of reports. Each report is made up of numbers called levels.

We want to figure out how many reports in total are considered to be safe.

We are told that a report is considered to be safe only if both of the following are true:

1. The levels are either all increasing or all decreasing.
2. Any two adjacent levels differ by at least one and at most three.

## Idea

For Part 1 we can implement a linear scan that checks, for each report, if both of the conditions above hold.

That is, we check for both:

1. **Monotonicity:** the numbers must strictly trend in a single direction, either all increasing or strictly decreasing.
2. **Gradual variance:** the absolute difference between any two neighbouring levels must be at least 1 and at most 3.

We can process each report and check for these conditions by using two boolean flags `increasing` and `decreasing` to track the direction, which are initially set to true.

For each pair of levels in the report we determine their difference `diff` and update the boolean flags to reflect this: set `decreasing` to false if `diff > 0`, and set `increasing` to false if `diff < 0`.

We also use the absolute value of this difference to check that the variance between the levels is within the given constraints.

A report is immediately found to be unsafe if this variance is not within the limits, or if at the end the boolean expression `increasing OR decreasing` evaluates to false, since a safe report can only be either increasing or decreasing, but not both.

## Pseudocode

Assuming `R[0..m]` contains a single report read from the input file and that `A[0..n]` contains all the reports read from the file, we can check if a single report `A[i] = R[0..m]` is safe using a helper function `checkIfSafe(R)`.

We can then simply count the number of reports read from the input file which are considered to be safe.

```text
checkIfSafe(R)
    m = R.size

    if m < 2
        return true

    increasing = true
    decreasing = true

    for i = 0 to m - 2
        diff = R[i + 1] - R[i]

        // check variance
        if |diff| < 1 or |diff| > 3
            return false

        // track directions
        if diff > 0
            decreasing = false

        if diff < 0
            increasing = false

    return increasing or decreasing


solvePart1(A)
    n = A.size
    total = 0

    for i = 0 to n - 1
        if checkIfSafe(A[i]) == true
            total = total + 1

    return total
```

## Complexity

* **Time Complexity:** $\mathcal{O}(n \times d)$  
  Where $n$ is the total number of reports (lines) in the puzzle input, and $d$ is the number of levels per report. We process the input lines sequentially, making a single linear scan of length $d$ for each line to calculate adjacent differences.

* **Space Complexity:** `O(n × d)` for storing all reports read from the input.  
  The safety check itself uses only `O(1)` auxiliary space.

---

## Part 2

In Part 2 we are told about the "Problem Dampener", which allows each report to tolerate up to one bad level, so that now a report that was previously considered unsafe in Part 1 could possibly be safe.

The rules for what makes a report safe are the same as Part 1, only that now we also consider previously unsafe reports to be safe if removing a single level turns them into a safe report.

## Idea

Since we are dealing with a relatively small input size, a brute-force approach of checking all unsafe reports by sequentially removing exactly one level would work.

We can re-use the logic used in Part 1's solution.

If a report is already safe, then we simply skip it, while for unsafe reports we exhaustively try all individual removals.

For each unsafe report, we loop through it and try removing one level at a time by constructing a copy of the report containing every level except the one we removed. We can pass this copy to our validation helper from Part 1 to see if the report is now safe.

If any removal results in an unsafe report becoming safe, we count it as safe.

If we try all individual removals and none of them pass the validation, then the report is considered to be unsafe.

## Pseudocode

Assuming `R[0..m]` contains a single report read from the input file and that `A[0..n]` contains all the reports read from the file, we can check if a single previously unsafe report `A[i] = R[0..m]` can now be considered safe by using the "Problem Dampener" logic.

The helper `checkIfSafeWithDampener(R)` does this by using the same helper `checkIfSafe(R)` used in Part 1.

We can then iterate through all the reports, applying the new "Problem Dampener" logic to count and return the new total number of safe reports for Part 2's solution.

```text
checkIfSafeWithDampener(R)

    // skip if already safe
    if checkIfSafe(R) == true
        return true

    m = R.size

    // try all individual removals
    for i = 0 to m - 1
        allocate copy_R[0..m - 1]

        // copy all levels except for current one being removed
        for j = 0 to m - 1
            if j != i
                append R[j] to copy_R

        // check if removal results in safe report
        if checkIfSafe(copy_R) == true
            return true

    // if all individual removals failed, report is unsafe
    return false


solvePart2(A)
    n = A.size
    total = 0

    for i = 0 to n - 1
        if checkIfSafeWithDampener(A[i]) == true
            total = total + 1

    return total
```

## Complexity

* **Time Complexity:** $\mathcal{O}(n \times d^2)$  
  Where $n$ is the total number of reports, and $d$ is the number of levels per report. For an unsafe report, we simulate at most $d$ removals. During these simulated removals, allocating and copying the remaining elements takes $\mathcal{O}(d)$ time, and validating after the removal using `checkIfSafe` takes $\mathcal{O}(d)$ time. This results in quadratic $\mathcal{O}(d^2)$ time per report.

* **Space Complexity:** `O(n × d)` for storing all reports read from the input.  
  During each simulated removal, the algorithm also creates a temporary copy of one report, which costs `O(d)` auxiliary space.


## Files

- `main.cpp` — reads the input file and prints both answers
- `day05.h` — declarations for `Report` and `RedNosedReports`
- `day05.cpp` — input parsing, report validation, validation applying problem dampener, and solution logic

## Build

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp day02.cpp -o day02
```

## Run

```bash
./day02
```

