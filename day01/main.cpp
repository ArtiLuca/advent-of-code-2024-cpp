#include "day01.h"

#include <iostream>
#include <exception>

int main() {

    try {
        ChiefHistorian history{"input.txt"};
        history.readPuzzleInput();

        std::cout << "--- Day 1: Historian Hysteria ---\n";
        std::cout << "Part 1: " << history.solvePart1() << '\n';
        std::cout << "Part 2: " << history.solvePart2(true) << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
};

/*
Part 1 Solution = 1530215
Part 2 Solution = 26800609
*/
