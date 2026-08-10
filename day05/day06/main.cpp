#include "day06.h"

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

int main() {

    try {
        Patrol layout{"input.txt"};
        layout.readPuzzleInput();

        std::cout << "--- Day 6: Guard Gallivant ---\n";
        std::cout << "Part 1: " << layout.solvePart1() << '\n';
        std::cout << "Part 2: " << layout.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

