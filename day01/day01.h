#pragma once

#include <string>
#include <vector>

class ChiefHistorian {

public:

    ChiefHistorian(const std::string& input) : puzzleInput(input) {}

    void readPuzzleInput();

    long long solvePart1();
    long long solvePart2(bool usingHash = false);

private:

    // two different approaches for solving Part 2
    long long solvePart2_BinarySearch();
    long long solvePart2_HashMap();

    // Binary search boundaries (pass the sorted copy by const reference)
    int findFirst(const std::vector<long long>& sortedRight, long long num);
    int findLast(const std::vector<long long>& sortedRight, long long num);

    std::string puzzleInput;
    std::vector<long long> leftList;
    std::vector<long long> rightList;
};