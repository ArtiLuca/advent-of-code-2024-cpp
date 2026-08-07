#include "day05.h"

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

int main() {

    try {
        PrintQueue printer{"input.txt"};
        printer.readPuzzleInput();

        std::cout << "--- Day 5: Print Queue ---\n";
        std::cout << "Part 1: " << printer.solvePart1() << '\n';
        std::cout << "Part 2: " << printer.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

// g++ -std=c++20 -Wall -Wextra -pedantic main.cpp day05.cpp -o day05