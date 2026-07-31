#include "day02.h"

#include <iostream>
#include <exception>
#include <stdexcept>

int main() {

    try {
        RedNosedReports rednose{"input.txt"};
        rednose.readPuzzleInput();

        std::cout << "--- Day 2: Red-Nosed Reports ---\n";
        std::cout << "Part 1: " << rednose.solvePart1() << '\n';
        std::cout << "Part 2: " << rednose.solvePart2() << '\n';
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

/* --- Output --- 

--- Day 9: Movie Theater ---                 
Part 1: 341
Part 2: 404

*/