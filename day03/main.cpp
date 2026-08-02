#include "day03.h"

#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        Program program{"input.txt"};
        program.readPuzzleInput();

        std::cout << "--- Day 3: Mull It Over ---\n";
        std::cout << "Part 1: " << program.solvePart1() << '\n';

        program.readPuzzleInputPart2();
        std::cout << "Part 2: " << program.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
