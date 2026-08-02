#pragma once

#include <string>
#include <vector>

static const std::string MUL_OP = "mul("; 
static const std::string DO_OP = "do()";
static const std::string DONT_OP = "don't()";
static const char SEPARATING_COMMA = ',';
static const char CLOSING_PARENTHESIS = ')';

struct Instruction {

    long long multiplicand;
    long long multiplier;

    // added for Part 2
    bool enabled = true;
};

class Program {

public:

    Program(const std::string& input) : puzzleInput(input) {}

    // read puzzle input using stream buffer
    void readPuzzleInput();
    // multiply all valid operations and return total sum
    long long solvePart1() const;

    // read puzzle input using stream buffer, and applying enable/disable logic
    void readPuzzleInputPart2();
    // multiply all valid and enabled operations and return total sum
    long long solvePart2() const;

private:

    // Private helper method for parsing a single operation
    bool parseNextInstruction(size_t& pos, Instruction& istr);

    std::string puzzleInput;
    std::string memory;
    std::vector<Instruction> operations;
};