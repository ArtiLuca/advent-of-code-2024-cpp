# Advent of Code 2024 - Day 1: Historian Hysteria

## Part 1

In Part 1, we are given two lists of location IDs.

The goal is to pair the smallest remaining value from the left list with the
smallest remaining value from the right list, then compute the distance between
the two values. The final answer is the sum of all these distances.

## Idea

Since we always need to compare the smallest remaining values from both lists,
we can sort both lists in increasing order.

After sorting, the smallest value in the left list is paired with the smallest
value in the right list, the second-smallest with the second-smallest, and so on.

For each pair, we add the absolute difference to a running total.

## Pseudocode

```text
solvePart1(leftList, rightList)
    sort leftList in increasing order
    sort rightList in increasing order

    total = 0

    for i = 0 to leftList.length - 1
        total = total + abs(leftList[i] - rightList[i])

    return total
```

## Complexity

Let `N` be the number of IDs in each list.

Sorting both lists dominates the running time:

```text
T(N) = Θ(N log N)
```

The final scan is linear, so it does not change the asymptotic bound.

The auxiliary space complexity is:

```text
Θ(N)
```

because my implementation creates sorted copies of the two input lists instead
of modifying the original stored input directly.

---

## Part 2

In Part 2, we no longer compare sorted pairs.

Instead, each number in the left list contributes a similarity score equal to:

```text
number * frequency of that number in the right list
```

The answer is the sum of these similarity scores over all numbers in the left
list.

For example, if the number `7` appears `3` times in the right list, then each
`7` in the left list contributes:

```text
7 * 3
```

to the total similarity score.

## Idea

I implemented two approaches for this part.

The first uses binary search on a sorted copy of the right list. This was useful
because it let me practice finding the first and last occurrence of a value in a
sorted array.

The second uses a hash map, which is simpler and more efficient on average.

In the final run, I use the hash map version.

---

## Approach 1: Binary Search

For the binary search approach, only the right list needs to be sorted.

After sorting the right list, all equal values are grouped together in one
contiguous block. For each number in the left list, I use two modified binary
searches:

- `findFirst`, which finds the first occurrence of the number in the right list;
- `findLast`, which finds the last occurrence of the number in the right list.

If the number exists in the right list, then its frequency is:

```text
lastIndex - firstIndex + 1
```

Then I add:

```text
number * frequency
```

to the total.

## Pseudocode

```text
findFirst(rightList, target)
    left = 0
    right = rightList.length - 1
    result = -1

    while left <= right
        mid = floor((left + right) / 2)

        if rightList[mid] == target
            result = mid
            right = mid - 1

        else if rightList[mid] < target
            left = mid + 1

        else
            right = mid - 1

    return result


findLast(rightList, target)
    left = 0
    right = rightList.length - 1
    result = -1

    while left <= right
        mid = floor((left + right) / 2)

        if rightList[mid] == target
            result = mid
            left = mid + 1

        else if rightList[mid] < target
            left = mid + 1

        else
            right = mid - 1

    return result


solvePart2_BinarySearch(leftList, rightList)
    sort rightList in increasing order

    total = 0

    for number in leftList
        firstIndex = findFirst(rightList, number)

        if firstIndex != -1
            lastIndex = findLast(rightList, number)
            frequency = lastIndex - firstIndex + 1
            total = total + number * frequency

    return total
```

## Complexity

Sorting the right list takes:

```text
Θ(N log N)
```

For each of the `N` numbers in the left list, we perform two binary searches,
each costing:

```text
O(log N)
```

Therefore, the total running time is:

```text
Θ(N log N)
```

The auxiliary space complexity is:

```text
Θ(N)
```

because the implementation creates copies of the lists before processing them.

---

## Approach 2: Hash Map

For the hash map approach, we avoid sorting entirely.

We first build a frequency table for the right list. Each key is a number from
the right list, and each value is the number of times it appears.

Then, for each number in the left list, we look up its frequency in the map and
add the corresponding similarity score.

## Pseudocode

```text
solvePart2_HashMap(leftList, rightList)
    create empty frequencyMap

    for number in rightList
        frequencyMap[number] = frequencyMap[number] + 1

    total = 0

    for number in leftList
        if number exists in frequencyMap
            frequency = frequencyMap[number]
        else
            frequency = 0

        total = total + number * frequency

    return total
```

## Complexity

Building the frequency map requires one scan of the right list.

Computing the total similarity score requires one scan of the left list.

Assuming average constant-time hash map operations, the running time is:

```text
Θ(N)
```

The auxiliary space complexity is:

```text
Θ(N)
```

in the worst case, when all values in the right list are distinct.

---

## Notes

Part 1 is naturally solved by sorting both lists and comparing them pairwise.

For Part 2, the binary search version was useful practice, but the hash map
version is the cleaner solution: it directly represents the idea of counting how
many times each number appears in the right list.

The program currently calls the hash map version for Part 2.

---

## Build

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp day01.cpp -o day01
```

## Run

```bash
./day01
```

The program expects the puzzle input in a file named `input.txt`.

---

## Files

```text
day01/
├── README.md
├── original_notes.md
├── main.cpp
├── day01.cpp
├── day01.h
└── input.txt          not included
```
