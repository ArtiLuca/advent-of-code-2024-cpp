#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/* Represents single page ordering rule */
struct Rule {
    
    int before;
    int after;
};

/* Represents a single update sequence */
struct Update {

    std::vector<int> pages;

    // helper that returns middle-point of update
    int getMiddlePoint() const;
};


class PrintQueue {

public:

    PrintQueue(const std::string& input) : puzzleInput(input) {}

    // reads puzzle input while parsing page ordering rules and update sequences
    void readPuzzleInput();

    // returns sum of all middle-points of only correctly-ordered updates
    int solvePart1() const;
    // returns sum of middle-points for all incorrectly-ordered updates, after sorting them so that they become correctly ordered
    int solvePart2() const;

private:

    // returns true if update sequence is correctly ordered
    bool isCorrectlyOrdered(const Update& update) const;

    std::string puzzleInput;
    std::unordered_map<int, std::unordered_set<int>> rules;
    std::vector<Update> updates;
};