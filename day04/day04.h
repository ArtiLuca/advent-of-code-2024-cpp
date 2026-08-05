#pragma once

#include <string>
#include <vector>

class WordSearch {

public:

    WordSearch(const std::string& input) : puzzleInput(input) {}
    
    // reads puzzle input and applies padding
    void readPuzzleInput();

    // returns total occurrences of "XMAS" in word search
    int solvePart1() const;

    // returns total number of valid "X-MAS" found in word search
    int solvePart2() const;

    // directional arrays
    static const int dr[];
    static const int dc[];

    // target string
    static const std::string target;

private:

    std::string puzzleInput;
    std::vector<std::string> grid;

};