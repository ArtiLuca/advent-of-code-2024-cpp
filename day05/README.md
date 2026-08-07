# Advent of Code 2024 - Day 5: Print Queue

## Part 1

In Part 1 we are tasked with helping to print out the sleigh launch safety manuals.

To correctly operate the printer, we are given as input the page ordering rules and the pages to produce in each update.

The page ordering rules are given using the notation `X|Y` to indicate that, if both page `X` and page `Y` are to be produced as part of an update, page number `X` must be printed at some point before page `Y`.

We are also given a list that specifies which pages are needed for each update, and since most safety manuals are different, the pages needed in the updates are different too.

We need to start by identifying which updates are already in the right order.

We must then calculate the middle point of all of the correctly ordered updates and sum them up in order to get Part 1's solution.

An example is shown below:

```text
47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47
```

In this example only the first three updates are correctly ordered, and their middle points correspond to the values 61, 53, and 29, which sum up to 143.

For Part 1's solution we are asked to determine all of the correctly ordered updates in order to sum up all of their middle points.

## Idea

For Part 1 we can make use of two helper structs `Rule` and `Update`, where a `Rule` indicates a page ordering rule such as `X|Y`, while `Update` indicates the list of page numbers needed for a single update.

Once we read these from input we can build a "lookup system" of sorts, where we store the rules in an associative array mapping a page number to a set of pages that must strictly follow it.

For example, in C++ we can use:

```text
std::unordered_map<int, std::unordered_set<int>>
```

We can then find all the correctly ordered updates by checking for any rule violations.

For any page at a given index `i`, we inspect all subsequent pages at positions `j > i`.

If we find that our rule map contains a rule stating that the page at index `j` should actually come before the one at index `i`, then we can immediately conclude that the entire update sequence is not correctly ordered.

Once we have found all correctly ordered updates, we can then find their middle point and sum all of these up for Part 1's solution.

## Pseudocode

Assuming we have correctly parsed all input page ordering rules and updates, and that we have mapped each page number to a set of pages that must follow it in a lookup table `rules` of type `(int -> set of int)`, we can check if a given update sequence `U` is valid using a helper function:

```text
correctlyOrdered(U)
    P = U.length

    // check all pairs for page-ordering violations
    for i = 0 to P - 1
        for j = i + 1 to P - 1
            page_a = U[i]
            page_b = U[j]

            // if page_b must precede page_a, the update is not correctly ordered
            if rules[page_b] contains page_a
                return false

    // if no violations were found, update is correctly ordered
    return true
```

We can then use this validation helper to aggregate the middle page values of all valid updates.

Assuming `updates` is an array containing all update sequences extracted from the input file:

```text
solvePart1(updates)
    total = 0
    L = updates.length

    for i = 0 to L - 1
        if correctlyOrdered(updates[i]) == true
            current_update = updates[i]
            middle_index = current_update.length / 2
            total = total + current_update[middle_index]

    return total
```

## Complexity

The **Time Complexity** is `O(R + L × P²)`.

In the parsing/setup phase we process `R` rules sequentially, and inserting elements into a hash table costs `O(1)` average time per rule.

During the validation phase, for each of the `L` updates we use a nested loop to check all pairs of pages.

A sequence of length `P` produces  

$\frac{P(P - 1)}{2}$  

unique pairs, and looking up each pair in our rule map has an average cost of `O(1)`.

In the worst case, where an update is completely valid, we perform `O(P²)` operations in total per update sequence.

The **Space Complexity** is `O(R + L × P)` since the hash structure uses `O(R)` space proportional to the number of page ordering rules, and we also store all the update sequences using `O(L × P)` space.

---

## Part 2

In Part 2 we are told to focus only on the previously incorrectly ordered updates and to sort these correctly using the page ordering constraints given to us initially.

Once we correctly order all of them, we want to determine the middle point of all the now correctly ordered updates and sum them all up for Part 2's solution.

## Idea

We can use a custom comparison function to order all the previously incorrectly ordered updates.

This can be implemented in C++ using `std::sort`.

We can use a custom comparator when comparing any two pages `a` and `b` and check if `b` is registered in the set of pages that must strictly follow `a`.

If this is true, then the lambda returns `true` to indicate that `a` must precede `b`.

We can use this comparator with `std::sort` to order all the incorrectly ordered update sequences while following the page ordering rules given to us.

Once a previously incorrectly ordered sequence is sorted, we can then calculate its middle point and add it to the total.

## Pseudocode

Assuming `updates` is the array of all input updates, and `rules` is our validated lookup structure:

```text
solvePart2(updates)
    total = 0
    L = updates.length

    for i = 0 to L - 1
        current_update = updates[i]

        // only process previously incorrectly ordered update sequences
        if correctlyOrdered(current_update) == false

            // custom sort using rule map as comparator
            // lambda(a,b) returns true if a must precede b
            sort current_update using lambda(a,b)
            {
                if rules[a] contains b
                    return true
                else
                    return false
            }

            // once it is correctly ordered, calculate middle point
            middle_index = current_update.length / 2
            total = total + current_update[middle_index]

    return total
```

## Complexity

The **Time Complexity** for Part 2 is `O(R + L × P²)`.

As in Part 1, building the rule map costs `O(R)`.

Then, for each update, we first check whether it is correctly ordered. This validation step costs `O(P²)` per update in the worst case.

For every incorrectly ordered update, we also sort the sequence. Sorting one update of length `P` costs `O(P log P)` comparisons, and each comparison performs an average `O(1)` lookup in the rule map.

Therefore, for each update, the validation cost `O(P²)` dominates the sorting cost `O(P log P)`.

Across all `L` updates, the total worst-case time complexity is therefore:

```text
O(R + L × P²)
```

The **Space Complexity** for Part 2 is the same as in Part 1, evaluating to `O(R + L × P)`.

The rule map stores the ordering rules, and the input updates are stored in memory. During sorting, the implementation also creates a temporary copy of each incorrectly ordered update, which costs `O(P)` auxiliary space for the update currently being processed.

---

## Build

```bash
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp day05.cpp -o day05
```

## Run

```bash
./day05
```

The program expects the puzzle input in a file named `input.txt`.

---

## Files

- `main.cpp` — creates the `PrintQueue` solver, reads the input file, and prints both answers
- `day05.h` — declares the `Rule` and `Update` structs and the `PrintQueue` class
- `day05.cpp` — handles input parsing, rule-map construction, update validation for Part 1, and sorting of incorrectly ordered updates for Part 2
