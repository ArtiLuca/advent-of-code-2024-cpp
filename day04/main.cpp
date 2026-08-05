#include "day04.h"

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

int main() {

    try {
        WordSearch wordsearch{"input.txt"};
        wordsearch.readPuzzleInput();

        std::cout << "--- Day 4: Ceres Search ---\n";
        std::cout << "Part 1: " << wordsearch.solvePart1() << '\n';
        std::cout << "Part 2: " << wordsearch.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}