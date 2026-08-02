# Advent of Code 2024 - Day 3: Mull It Over

## Part 1

In Part 1 we are scanning a program's corrupted memory for all of its valid multiplication operations.

A multiplication operation always starts with the op-word `mul` and is then immediately followed by `(X,Y)`, where `X` and `Y` are valid numbers corresponding to the multiplicand and multiplier of a given multiplication operation.

However, the memory is corrupted, so not all multiplication operations are valid.

Therefore, we must parse all the valid ones, multiply them, and sum all of them up in order to get Part 1's solution.

## Idea

A valid multiplication operation is always in the form of the string `mul` followed by an opening parenthesis, the multiplicand, the separating comma, the multiplier, and then the closing parenthesis.

The first thing that came to mind was using a regex; however, I have not used them much, so I decided to implement a first solution using `std::string::find`, specifically using it to search for each occurrence of the string `"mul("` in the program's corrupted memory.

We can parse all valid multiplications from the program's corrupted memory by repeatedly finding the next occurrence of `"mul("`.

If none is found, it means we have reached the end of the program's memory.

At each occurrence found, we take the position found and:

1. increment this position by 4 since `length("mul(") == 4`;
2. extract the multiplicand by extracting the following digit, or digits if it is a multi-digit value, and then check for the comma-separating character. If the multiplicand has no digits, we went past the memory's total length, or the comma-separating character was not found, we then move on to the next occurrence since we were not able to parse a valid operation;
3. if we managed to parse the multiplicand and found the comma-separating value, we then repeat the same process for the multiplier and check for the closing parenthesis character. If the multiplier has no digits, we went past the memory's total length, or no closing parenthesis was found, we then move on to the next occurrence;
4. if both the multiplicand and multiplier were extracted, then we have a valid operation. We convert the multiplicand and multiplier into their corresponding numerical values by repeatedly multiplying the sum, initially set to 0, by 10 and adding the value of each digit, meaning we do: `sum = sum * 10 + (c - '0')`.

Once we have successfully parsed a valid multiplication operation, we can store it in an empty list to later multiply and sum up to solve Part 1.

In C++, `std::string::find` returns the starting position of the occurrence if it was found; otherwise, it returns `std::string::npos`.

We can also use `std::isdigit` for the extraction of the multiplicand and multiplier and can use two temporary variables `pos` and `curr` to keep track of where we are within the program's corrupted memory.

## Pseudocode

Assuming that `memory[0..n]` contains the entire program's memory, that the operation `memory[pos..pos+3] == "mul("` behaves the same way as `std::string::find`, and that each operation is seen as an `Instruction` data structure with fields `multiplicand` and `multiplier`:

```text
readPuzzleInput(memory)
    n = length(memory)
    pos = 0
    allocate operations as empty list

    // scan the entire program's memory
    while pos < n - 3

        // find next multiplication operation to validate
        if memory[pos..pos+3] == "mul("
            // check if we can parse using helper
            create I as new Instruction

            if parseNextInstruction(pos, I, n) == true
                append I to operations list
            else
                pos = pos + 1

        else
            pos = pos + 1
```

Assuming `pos` and `instr` are passed by reference, where `pos` indicates the current position and `instr` is the `Instruction` we are currently parsing:

```text
parseNextInstruction(pos, instr, n)
    curr = pos + 4
    allocate value_x as empty string

    // parse multiplicand's digits
    while curr < n and memory[curr] is a digit
        append memory[curr] to value_x
        curr = curr + 1

    // check if parsing was successful
    if length(value_x) == 0 or curr >= n or memory[curr] != ','
        return false

    // otherwise, we skip comma
    curr = curr + 1

    allocate value_y as empty string

    // parse multiplier's digits
    while curr < n and memory[curr] is a digit
        append memory[curr] to value_y
        curr = curr + 1

    // check if parsing was successful
    if length(value_y) == 0 or curr >= n or memory[curr] != ')'
        return false

    // if we arrive here we successfully extracted a valid operation

    // extract multiplicand
    x = 0
    for i = 0 to length(value_x) - 1
        x = x * 10 + (value_x[i] - '0')

    // extract multiplier
    y = 0
    for j = 0 to length(value_y) - 1
        y = y * 10 + (value_y[j] - '0')

    // successfully parsed an operation
    instr.multiplicand = x
    instr.multiplier = y

    // update the reference position to point past the closing parenthesis
    pos = curr + 1

    return true
```

```text
solvePart1(operations)
    total = 0

    for each I in operations
        total = total + (I.multiplicand * I.multiplier)

    return total
```

## Notes

Initially, I naively thought that the entire puzzle input was one single line, so I simply used `std::getline`, but this did not work.

However, since the puzzle input actually contains `\n` characters, I changed the initial read function to use `std::istreambuf_iterator<char>` in order to read into `memory` every single character from the input, treating the entire input as one continuous block of text.

Also, to improve readability, in the actual C++ implementation I delegate the process of parsing and checking each possible multiplication instruction to a dedicated helper `parseNextInstruction(pos, instr)`, which takes `pos` and `instr` by reference and returns true if the operation was parsed successfully and false otherwise.

## Complexity

The **Time Complexity** is `O(N)`, where `N` is the total number of characters that make up the program's corrupted memory.

The `std::string::find` method is efficient for this use case, and the search position only moves forward through the input. When it finds a `"mul("` occurrence, the parsing helper performs work proportional to the size of the instruction being parsed.

The **Space Complexity** is `O(N + K)`, where `N` is the number of characters loaded into `memory` from the puzzle input, and `K` is the number of valid multiplication operations parsed from the input.

Since `K` is bounded by the size of the input, we can simplify the space complexity by saying it is `O(N)`.

---

## Part 2

In Part 2 we are told to consider two other types of operations that may appear in the corrupted memory.

These operations are `do()` and `don't()`, which respectively **enable** and **disable** multiplication operations.

Only the most recent `do()` or `don't()` instruction applies and, at the very beginning of the program, `mul` operations are enabled by default.

For Part 2's solution, we must multiply and sum all the multiplication instructions while applying these new rules to check whether they are enabled or disabled.

## Idea

We already have the logic for parsing all valid multiplication operations from the input, so we can easily adapt these new enable/disable rules by modifying the initial reading logic used in Part 1.

For each instruction, we add a boolean flag `enabled` that is set to false if we encountered a `don't()` instruction prior to it, and set to true if we encountered a `do()` instruction prior to it.

We can adapt the `readPuzzleInput()` procedure from Part 1 to handle these new rules, which for readability I re-implemented in a separate `readPuzzleInputPart2()` procedure.

Since the `do()` and `don't()` operations act as a simple **state machine** with an initial value set to true, we can update the reading logic by searching the program's memory for all three instructions: `"mul("`, `"do()"` and `"don't()"`.

In particular, we find the one with the minimum position, meaning the one we encounter first chronologically:

- if the minimum is a `do()` operation, then we set the state machine's value to true;
- if the minimum is a `don't()` operation, we set the state machine's value to false;
- if it is a `mul(` operation, then we parse the multiplication operation using the same `parseNextInstruction` helper used for Part 1, assign it the current state machine's value, and push it into our list of operations;
- if the minimum is `std::string::npos`, then we have reached the end of the program's memory, at which point the loop terminates.

## Pseudocode

Under the same assumptions used in Part 1, and also assuming that now each `Instruction` has a boolean `enabled` value to indicate enabled/disabled status.

The only changes we must make are in the initial input reading logic and a small check in the final calculation of Part 2's solution.

```text
readPuzzleInputPart2(memory)
    n = length(memory)
    pos = 0
    allocate operations as empty list

    // set initial state value (default is true)
    is_enabled = true

    // scan the entire program's memory
    while pos < n - 3

        // find position of all three instructions
        next_do = index of next "do()" starting from pos
        next_dont = index of next "don't()" starting from pos
        next_mul = index of next "mul(" starting from pos

        // find the one that appears first
        next = min(next_do, next_dont, next_mul)

        // if none were found, we finished
        if next not found
            break

        // otherwise, handle instruction that appeared first
        if next == next_do
            is_enabled = true
            pos = next_do + 4

        else if next == next_dont
            is_enabled = false
            pos = next_dont + 7

        else if next == next_mul
            pos = next_mul
            create I as new Instruction

            // check if we can parse using helper
            if parseNextInstruction(pos, I, n) == true
                I.enabled = is_enabled
                append I to operations list
            else
                pos = pos + 1

    return operations
```

```text
solvePart2(operations)
    total = 0

    for each I in operations
        // only sum multiplications that are enabled
        if I.enabled == true
            total = total + (I.multiplicand * I.multiplier)

    return total
```

## Complexity

The **Time Complexity** and **Space Complexity** remain the same as they were in Part 1.

The only difference is that, while parsing, we also track a boolean state representing whether multiplication instructions are currently enabled or disabled.

---

## Build

```bash
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp day03.cpp -o day03
```

## Run

```bash
./day03
```

The program expects the puzzle input in a file named `input.txt`.

---

## Files

- `main.cpp` — creates the `Program` solver, reads the input file, and prints both answers
- `day03.h` — declares the `Instruction` struct, the operation tokens, and the `Program` class
- `day03.cpp` — handles input parsing, multiplication parsing for Part 1, enable/disable parsing for Part 2, and the final summing logic
